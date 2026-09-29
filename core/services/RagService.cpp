#include "core/services/RagService.h"

#include <QStringList>

namespace winccmcp::core::services {

namespace {

const QString kSystemPrompt = QStringLiteral(
    "You are an assistant for the SIMATIC WinCC OA documentation (a Siemens SCADA system). "
    "Answer the question using ONLY the documentation excerpts in the context below. "
    "If the context doesn't contain the answer, honestly say it wasn't found in the "
    "documentation - don't invent functions, parameters, or behavior. "
    "After every statement, cite the source as [N], where N is the excerpt number. "
    "Answer in the language of the question.");

QString buildContext(const models::RetrievedChunks& chunks) {
    QStringList parts;
    parts.reserve(chunks.size());
    for (int i = 0; i < chunks.size(); ++i) {
        const auto& c = chunks.at(i);
        parts << QStringLiteral("[%1] Source: %2 (%3)\n%4")
                     .arg(i + 1)
                     .arg(c.documentPath, c.documentTitle, c.text);
    }
    return parts.join(QStringLiteral("\n\n---\n\n"));
}

} // namespace

RagService::RagService(interfaces::IEmbeddingClient& embeddingClient,
                        interfaces::ILlmChatClient& chatClient,
                        interfaces::IVectorStore& vectorStore,
                        interfaces::IDocumentRepository& documentRepository,
                        int defaultTopK)
    : m_embeddingClient(embeddingClient)
    , m_chatClient(chatClient)
    , m_vectorStore(vectorStore)
    , m_documentRepository(documentRepository)
    , m_defaultTopK(defaultTopK) {
}

models::RetrievedChunks RagService::retrieve(const QString& query, int topK) {
    const int k = topK > 0 ? topK : m_defaultTopK;

    const auto embeddings = m_embeddingClient.embed({query});
    if (embeddings.isEmpty()) {
        return {};
    }

    const auto matches = m_vectorStore.searchNearest(embeddings.first(), k);
    return m_documentRepository.resolveChunks(matches);
}

models::RagAnswer RagService::answer(const QString& question, int topK) {
    const auto chunks = retrieve(question, topK);
    if (chunks.isEmpty()) {
        return models::RagAnswer{
            QStringLiteral("Nothing was found in the documentation database. The index may not have been built yet."),
            {}};
    }

    const models::ChatTurns messages{
        models::ChatTurn{models::ChatRole::System, kSystemPrompt},
        models::ChatTurn{
            models::ChatRole::User,
            QStringLiteral("Documentation context:\n\n%1\n\nQuestion: %2")
                .arg(buildContext(chunks), question)},
    };

    const QString reply = m_chatClient.chat(messages);

    models::Sources sources;
    sources.reserve(chunks.size());
    for (const auto& c : chunks) {
        sources.push_back(models::Source{c.documentPath, c.documentTitle, c.distance});
    }

    return models::RagAnswer{reply, sources};
}

} // namespace winccmcp::core::services
