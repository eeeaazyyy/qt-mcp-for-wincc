#include "infrastructure/persistence/SqliteSchema.h"

#include <optional>
#include <stdexcept>

#include "infrastructure/persistence/SqliteStatement.h"

namespace winccmcp::infrastructure::persistence {

namespace {

std::optional<int> readStoredEmbeddingDim(SqliteConnection& connection) {
    connection.exec(QStringLiteral("CREATE TABLE IF NOT EXISTS meta (key TEXT PRIMARY KEY, value TEXT)"));

    SqliteStatement stmt(connection.handle(), QStringLiteral("SELECT value FROM meta WHERE key = 'embedding_dim'"));
    if (!stmt.step()) {
        return std::nullopt;
    }
    return stmt.columnText(0).toInt();
}

} // namespace

void SqliteSchema::ensureCreated(SqliteConnection& connection, int embeddingDim) {
    const auto existingDim = readStoredEmbeddingDim(connection);
    if (existingDim.has_value() && *existingDim != embeddingDim) {
        throw std::runtime_error(
            "The database was already created with embedding dimension " + std::to_string(*existingDim)
            + ", but the current embedding model produces " + std::to_string(embeddingDim)
            + ". Delete the database file and re-run indexing if you changed the embedding model.");
    }

    connection.exec(QStringLiteral(R"SQL(
        CREATE TABLE IF NOT EXISTS documents (
            id INTEGER PRIMARY KEY,
            path TEXT UNIQUE NOT NULL,
            title TEXT NOT NULL,
            shortdesc TEXT,
            text TEXT NOT NULL,
            content_hash TEXT NOT NULL
        )
    )SQL"));

    connection.exec(QStringLiteral(R"SQL(
        CREATE TABLE IF NOT EXISTS chunks (
            id INTEGER PRIMARY KEY,
            doc_id INTEGER NOT NULL REFERENCES documents(id) ON DELETE CASCADE,
            chunk_index INTEGER NOT NULL,
            text TEXT NOT NULL
        )
    )SQL"));

    connection.exec(QStringLiteral("CREATE VIRTUAL TABLE IF NOT EXISTS vec_chunks USING vec0(embedding float[%1])")
                         .arg(embeddingDim));

    connection.exec(QStringLiteral(R"SQL(
        CREATE TABLE IF NOT EXISTS chat_history (
            id INTEGER PRIMARY KEY,
            ts TEXT NOT NULL,
            query TEXT NOT NULL,
            answer TEXT NOT NULL,
            sources_json TEXT NOT NULL,
            duration_ms INTEGER NOT NULL
        )
    )SQL"));

    SqliteStatement setDim(connection.handle(),
                            QStringLiteral("INSERT OR REPLACE INTO meta (key, value) VALUES ('embedding_dim', ?)"));
    setDim.bind(1, QString::number(embeddingDim));
    setDim.step();
}

} // namespace winccmcp::infrastructure::persistence
