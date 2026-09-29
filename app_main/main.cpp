#include <cstdlib>
#include <memory>

#include <QApplication>
#include <QMessageBox>
#include <QProcessEnvironment>

#include "app/common/TranslationManager.h"
#include "app/mainwindow/MainWindow.h"
#include "core/services/IngestionService.h"
#include "core/services/McpToolRegistry.h"
#include "core/services/RagService.h"
#include "infrastructure/llm/LlmConfig.h"
#include "infrastructure/llm/LmStudioClient.h"
#include "infrastructure/mcp/HttpJsonRpcTransport.h"
#include "infrastructure/mcp/JsonRpcDispatcher.h"
#include "infrastructure/parsing/LexborHtmlParser.h"
#include "infrastructure/parsing/ParagraphChunker.h"
#include "infrastructure/persistence/SqliteChatHistoryRepository.h"
#include "infrastructure/persistence/SqliteConnection.h"
#include "infrastructure/persistence/SqliteDocumentRepository.h"
#include "infrastructure/persistence/SqliteEnvironment.h"
#include "infrastructure/persistence/SqliteSchema.h"
#include "infrastructure/persistence/SqliteVectorStore.h"

#ifndef WINCCMCP_DEFAULT_DOCS_DIR
#define WINCCMCP_DEFAULT_DOCS_DIR ""
#endif
#ifndef WINCCMCP_DEFAULT_DB_PATH
#define WINCCMCP_DEFAULT_DB_PATH "winccoa_docs.sqlite3"
#endif

namespace {

QString envOr(const QProcessEnvironment& env, const char* key, const QString& fallback) {
    return env.contains(QLatin1String(key)) ? env.value(QLatin1String(key)) : fallback;
}

} // namespace

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setOrganizationName(QStringLiteral("qt-mcp-for-wincc"));
    QApplication::setApplicationName(QStringLiteral("qt-mcp-for-wincc"));

    const auto env = QProcessEnvironment::systemEnvironment();
    const QString docsRootDir = envOr(env, "WINCCMCP_DOCS_DIR", QStringLiteral(WINCCMCP_DEFAULT_DOCS_DIR));
    const QString dbPath = envOr(env, "WINCCMCP_DB_PATH", QStringLiteral(WINCCMCP_DEFAULT_DB_PATH));
    const QString httpHost = envOr(env, "WINCCMCP_HTTP_HOST", QStringLiteral("127.0.0.1"));
    const int httpPort = envOr(env, "WINCCMCP_HTTP_PORT", QStringLiteral("8765")).toInt();

    using namespace winccmcp;

    infrastructure::persistence::SqliteEnvironment::initializeOnce();

    infrastructure::llm::LmStudioClient llmClient(infrastructure::llm::LlmConfig::fromEnvironment());

    int embeddingDim = 0;
    try {
        embeddingDim = llmClient.embeddingDimensions();
    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, QObject::tr("LM Studio is unavailable"),
                               QObject::tr("Failed to get an embedding from LM Studio:\n%1\n\n"
                                           "Check that LM Studio is running, Local Server is enabled, "
                                           "and an embedding model is loaded (see the LLM_* environment variables).")
                                   .arg(QString::fromUtf8(e.what())));
        return EXIT_FAILURE;
    }

    std::unique_ptr<infrastructure::persistence::SqliteConnection> connection;
    try {
        connection = std::make_unique<infrastructure::persistence::SqliteConnection>(dbPath);
        infrastructure::persistence::SqliteSchema::ensureCreated(*connection, embeddingDim);
    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, QObject::tr("Database error"), QString::fromUtf8(e.what()));
        return EXIT_FAILURE;
    }

    infrastructure::persistence::SqliteVectorStore vectorStore(*connection);
    infrastructure::persistence::SqliteDocumentRepository documentRepository(*connection);
    infrastructure::persistence::SqliteChatHistoryRepository historyRepository(*connection);

    infrastructure::parsing::LexborHtmlParser htmlParser;
    infrastructure::parsing::ParagraphChunker chunker;

    core::services::RagService ragService(llmClient, llmClient, vectorStore, documentRepository);
    core::services::IngestionService ingestionService(htmlParser, chunker, llmClient, vectorStore, documentRepository);

    core::services::McpToolRegistry toolRegistry(ragService, documentRepository);
    infrastructure::mcp::JsonRpcDispatcher dispatcher(toolRegistry);

    infrastructure::mcp::HttpJsonRpcTransport transport(httpHost, httpPort);
    transport.start(dispatcher);

    app::common::TranslationManager translationManager;

    app::mainwindow::MainWindow window(ragService, ingestionService, historyRepository, docsRootDir, translationManager);
    window.show();

    const int exitCode = QApplication::exec();

    transport.stop();
    return exitCode;
}
