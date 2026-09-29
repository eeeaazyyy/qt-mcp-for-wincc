#include "infrastructure/persistence/SqliteVectorStore.h"

#include "infrastructure/persistence/SqliteStatement.h"

namespace winccmcp::infrastructure::persistence {

using core::models::Embedding;
using core::models::VectorMatch;
using core::models::VectorMatches;

SqliteVectorStore::SqliteVectorStore(SqliteConnection& connection) : m_connection(connection) {
}

void SqliteVectorStore::insertChunkEmbedding(qint64 chunkId, const Embedding& embedding) {
    SqliteStatement stmt(m_connection.handle(),
                          QStringLiteral("INSERT INTO vec_chunks (rowid, embedding) VALUES (?, ?)"));
    stmt.bind(1, chunkId);
    stmt.bindBlob(2, embedding.constData(), static_cast<int>(embedding.size() * sizeof(float)));
    stmt.step();
}

void SqliteVectorStore::removeChunkEmbedding(qint64 chunkId) {
    SqliteStatement stmt(m_connection.handle(), QStringLiteral("DELETE FROM vec_chunks WHERE rowid = ?"));
    stmt.bind(1, chunkId);
    stmt.step();
}

VectorMatches SqliteVectorStore::searchNearest(const Embedding& queryEmbedding, int topK) {
    SqliteStatement stmt(m_connection.handle(), QStringLiteral(
        "SELECT rowid, distance FROM vec_chunks WHERE embedding MATCH ? AND k = ? ORDER BY distance"));
    stmt.bindBlob(1, queryEmbedding.constData(), static_cast<int>(queryEmbedding.size() * sizeof(float)));
    stmt.bind(2, static_cast<qint64>(topK));

    VectorMatches matches;
    while (stmt.step()) {
        matches.push_back(VectorMatch{stmt.columnInt64(0), stmt.columnDouble(1)});
    }
    return matches;
}

} // namespace winccmcp::infrastructure::persistence
