#include "core/services/IngestionService.h"

#include <algorithm>

#include <QDir>
#include <QDirIterator>
#include <QStringList>

namespace winccmcp::core::services {

IngestionService::IngestionService(interfaces::IHtmlDocumentParser& htmlParser,
                                    interfaces::ITextChunker& textChunker,
                                    interfaces::IEmbeddingClient& embeddingClient,
                                    interfaces::IVectorStore& vectorStore,
                                    interfaces::IDocumentRepository& documentRepository)
    : m_htmlParser(htmlParser)
    , m_textChunker(textChunker)
    , m_embeddingClient(embeddingClient)
    , m_vectorStore(vectorStore)
    , m_documentRepository(documentRepository) {
}

IngestionService::Result IngestionService::run(const QString& docsRootDir,
                                                 const ProgressCallback& onProgress) {
    Result result;

    QStringList files;
    QDirIterator it(docsRootDir, {QStringLiteral("*.html")}, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        files << it.next();
    }
    files.sort();

    const QDir root(docsRootDir);

    for (int i = 0; i < files.size(); ++i) {
        const QString& absolutePath = files.at(i);
        const QString relativePath = root.relativeFilePath(absolutePath);

        if (onProgress) {
            onProgress(i + 1, files.size(), relativePath);
        }

        const auto parsed = m_htmlParser.parseFile(absolutePath, relativePath);
        if (!parsed) {
            continue; // a utility page with no main content
        }
        ++result.parsedDocuments;

        const auto [documentId, changed] = m_documentRepository.upsertDocument(*parsed);
        if (!changed) {
            ++result.skippedUnchanged;
            continue;
        }

        const auto chunkTexts = m_textChunker.split(parsed->text);
        if (chunkTexts.isEmpty()) {
            continue;
        }

        for (const qint64 oldChunkId : m_documentRepository.chunkIdsForDocument(documentId)) {
            m_vectorStore.removeChunkEmbedding(oldChunkId);
        }

        const auto chunks = m_documentRepository.replaceChunks(documentId, chunkTexts);
        const auto embeddings = m_embeddingClient.embed(chunkTexts);

        const int n = std::min(chunks.size(), embeddings.size());
        for (int c = 0; c < n; ++c) {
            m_vectorStore.insertChunkEmbedding(chunks.at(c).id, embeddings.at(c));
        }

        ++result.reindexedDocuments;
        result.totalChunks += n;
    }

    return result;
}

} // namespace winccmcp::core::services
