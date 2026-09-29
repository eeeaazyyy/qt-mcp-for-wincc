#ifndef WINCCMCP_CORE_INTERFACES_IVECTORSTORE_H__C5D6E7F8_A96B_4B7C_E3AE_F5A6B7C8D9EA__INCLUDED_
#define WINCCMCP_CORE_INTERFACES_IVECTORSTORE_H__C5D6E7F8_A96B_4B7C_E3AE_F5A6B7C8D9EA__INCLUDED_

#include "core/models/Chunk.h"
#include "core/models/VectorMatch.h"

namespace winccmcp::core::interfaces {

// Vector database (Repository for embeddings). Default implementation -
// SqliteVectorStore on top of sqlite-vec, but anything can be substituted
// behind this interface (OCP) - RagService/IngestionService won't notice.
class IVectorStore {
public:
    virtual ~IVectorStore() = default;

    virtual void insertChunkEmbedding(qint64 chunkId, const models::Embedding& embedding) = 0;
    virtual void removeChunkEmbedding(qint64 chunkId) = 0;

    virtual models::VectorMatches searchNearest(const models::Embedding& queryEmbedding, int topK) = 0;
};

} // namespace winccmcp::core::interfaces

#endif // WINCCMCP_CORE_INTERFACES_IVECTORSTORE_H__C5D6E7F8_A96B_4B7C_E3AE_F5A6B7C8D9EA__INCLUDED_
