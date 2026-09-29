#ifndef WINCCMCP_CORE_INTERFACES_IDOCUMENTREPOSITORY_H__D6E7F8A9_B06C_4C8D_F4BF_A6B7C8D9EAFB__INCLUDED_
#define WINCCMCP_CORE_INTERFACES_IDOCUMENTREPOSITORY_H__D6E7F8A9_B06C_4C8D_F4BF_A6B7C8D9EAFB__INCLUDED_

#include <optional>
#include <utility>

#include <QString>
#include <QVector>

#include "core/models/Chunk.h"
#include "core/models/Document.h"
#include "core/models/RetrievedChunk.h"
#include "core/models/VectorMatch.h"

namespace winccmcp::core::interfaces {

// Storage for documents and their text chunks (the embeddings themselves live
// in IVectorStore - see the comment there about the split of responsibility).
class IDocumentRepository {
public:
    virtual ~IDocumentRepository() = default;

    virtual std::optional<models::Document> findByPath(const QString& path) = 0;

    // Substring in title/path, for list_documents.
    virtual QVector<models::Document> search(const QString& substring, int limit) = 0;

    // Returns {documentId, changed}. changed == false means the document's
    // content hasn't changed since the last ingest (by contentHash), so its
    // chunks/embeddings don't need to be recomputed.
    virtual std::pair<qint64, bool> upsertDocument(const models::Document& doc) = 0;

    // Ids of the document's existing chunks - IngestionService must remove
    // their vectors via IVectorStore::removeChunkEmbedding() BEFORE calling
    // replaceChunks(), otherwise vec_chunks ends up with orphaned rows
    // (vec0 has no ON DELETE CASCADE).
    virtual QVector<qint64> chunkIdsForDocument(qint64 documentId) = 0;

    // Deletes the document's old chunks and inserts new ones; returns the
    // chunks with their ids already assigned (needed for
    // IVectorStore::insertChunkEmbedding).
    virtual QVector<models::Chunk> replaceChunks(qint64 documentId, const QVector<QString>& chunkTexts) = 0;

    // Attaches chunk text and document metadata to KNN search results,
    // preserving order (nearest first).
    virtual models::RetrievedChunks resolveChunks(const models::VectorMatches& matches) = 0;
};

} // namespace winccmcp::core::interfaces

#endif // WINCCMCP_CORE_INTERFACES_IDOCUMENTREPOSITORY_H__D6E7F8A9_B06C_4C8D_F4BF_A6B7C8D9EAFB__INCLUDED_
