#ifndef WINCCMCP_INFRASTRUCTURE_PERSISTENCE_SQLITEVECTORSTORE_H__E9FA0B1C_2D89_499A_6122_7D8E9FA0B1C8__INCLUDED_
#define WINCCMCP_INFRASTRUCTURE_PERSISTENCE_SQLITEVECTORSTORE_H__E9FA0B1C_2D89_499A_6122_7D8E9FA0B1C8__INCLUDED_

#include "core/interfaces/IVectorStore.h"
#include "infrastructure/persistence/SqliteConnection.h"

namespace winccmcp::infrastructure::persistence {

// IVectorStore on top of the sqlite-vec virtual table (vec0). Requires that
// SqliteSchema::ensureCreated() has already created the vec_chunks table on
// this same connection.
class SqliteVectorStore : public core::interfaces::IVectorStore {
public:
    explicit SqliteVectorStore(SqliteConnection& connection);

    void insertChunkEmbedding(qint64 chunkId, const core::models::Embedding& embedding) override;
    void removeChunkEmbedding(qint64 chunkId) override;
    core::models::VectorMatches searchNearest(const core::models::Embedding& queryEmbedding, int topK) override;

private:
    SqliteConnection& m_connection;
};

} // namespace winccmcp::infrastructure::persistence

#endif // WINCCMCP_INFRASTRUCTURE_PERSISTENCE_SQLITEVECTORSTORE_H__E9FA0B1C_2D89_499A_6122_7D8E9FA0B1C8__INCLUDED_
