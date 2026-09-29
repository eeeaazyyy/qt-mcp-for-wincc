#ifndef WINCCMCP_CORE_SERVICES_INGESTIONSERVICE_H__E3F4A5B6_C723_4334_0BC6_1D2E3F4A5B62__INCLUDED_
#define WINCCMCP_CORE_SERVICES_INGESTIONSERVICE_H__E3F4A5B6_C723_4334_0BC6_1D2E3F4A5B62__INCLUDED_

#include <functional>

#include <QString>

#include "core/interfaces/IDocumentRepository.h"
#include "core/interfaces/IEmbeddingClient.h"
#include "core/interfaces/IHtmlDocumentParser.h"
#include "core/interfaces/ITextChunker.h"
#include "core/interfaces/IVectorStore.h"

namespace winccmcp::core::services {

// Walks the HTML documentation folder and builds/updates the index: the same
// pipeline as ingest.py in the Python version (parse -> chunk -> embed ->
// store), but expressed through interfaces rather than concrete modules.
class IngestionService {
public:
    // (current file, total files, current file's relative path) - invoked
    // from the GUI thread via QMetaObject::invokeMethod if run() executes on
    // a background thread (see app::chat::ChatController / MainWindow).
    using ProgressCallback = std::function<void(int current, int total, const QString& relativePath)>;

    struct Result {
        int parsedDocuments = 0;
        int skippedUnchanged = 0;
        int reindexedDocuments = 0;
        int totalChunks = 0;
    };

    IngestionService(interfaces::IHtmlDocumentParser& htmlParser,
                      interfaces::ITextChunker& textChunker,
                      interfaces::IEmbeddingClient& embeddingClient,
                      interfaces::IVectorStore& vectorStore,
                      interfaces::IDocumentRepository& documentRepository);

    Result run(const QString& docsRootDir, const ProgressCallback& onProgress = {});

private:
    interfaces::IHtmlDocumentParser& m_htmlParser;
    interfaces::ITextChunker& m_textChunker;
    interfaces::IEmbeddingClient& m_embeddingClient;
    interfaces::IVectorStore& m_vectorStore;
    interfaces::IDocumentRepository& m_documentRepository;
};

} // namespace winccmcp::core::services

#endif // WINCCMCP_CORE_SERVICES_INGESTIONSERVICE_H__E3F4A5B6_C723_4334_0BC6_1D2E3F4A5B62__INCLUDED_
