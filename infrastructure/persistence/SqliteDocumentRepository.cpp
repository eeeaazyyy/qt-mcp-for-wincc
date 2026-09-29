#include "infrastructure/persistence/SqliteDocumentRepository.h"

#include <QCryptographicHash>

#include "infrastructure/persistence/SqliteStatement.h"

namespace winccmcp::infrastructure::persistence {

using core::models::Chunk;
using core::models::Document;
using core::models::RetrievedChunk;
using core::models::RetrievedChunks;
using core::models::VectorMatches;

namespace {

QString sha256Hex(const QString& text) {
    return QString::fromLatin1(QCryptographicHash::hash(text.toUtf8(), QCryptographicHash::Sha256).toHex());
}

} // namespace

SqliteDocumentRepository::SqliteDocumentRepository(SqliteConnection& connection) : m_connection(connection) {
}

std::optional<Document> SqliteDocumentRepository::findByPath(const QString& path) {
    SqliteStatement stmt(m_connection.handle(),
                          QStringLiteral("SELECT id, path, title, shortdesc, text, content_hash FROM documents WHERE path = ?"));
    stmt.bind(1, path);
    if (!stmt.step()) {
        return std::nullopt;
    }

    Document doc;
    doc.id = stmt.columnInt64(0);
    doc.path = stmt.columnText(1);
    doc.title = stmt.columnText(2);
    doc.shortdesc = stmt.columnText(3);
    doc.text = stmt.columnText(4);
    doc.contentHash = stmt.columnText(5);
    return doc;
}

QVector<Document> SqliteDocumentRepository::search(const QString& substring, int limit) {
    SqliteStatement stmt(m_connection.handle(), QStringLiteral(
        "SELECT path, title, shortdesc FROM documents "
        "WHERE title LIKE ? OR path LIKE ? ORDER BY title LIMIT ?"));
    const QString like = QStringLiteral("%%%1%%").arg(substring);
    stmt.bind(1, like);
    stmt.bind(2, like);
    stmt.bind(3, static_cast<qint64>(limit));

    QVector<Document> result;
    while (stmt.step()) {
        Document doc;
        doc.path = stmt.columnText(0);
        doc.title = stmt.columnText(1);
        doc.shortdesc = stmt.columnText(2);
        result.push_back(doc);
    }
    return result;
}

std::pair<qint64, bool> SqliteDocumentRepository::upsertDocument(const Document& doc) {
    const QString hash = sha256Hex(doc.text);

    SqliteStatement find(m_connection.handle(), QStringLiteral("SELECT id, content_hash FROM documents WHERE path = ?"));
    find.bind(1, doc.path);

    if (find.step()) {
        const qint64 id = find.columnInt64(0);
        const QString existingHash = find.columnText(1);
        if (existingHash == hash) {
            return {id, false};
        }

        SqliteStatement update(m_connection.handle(), QStringLiteral(
            "UPDATE documents SET title = ?, shortdesc = ?, text = ?, content_hash = ? WHERE id = ?"));
        update.bind(1, doc.title);
        update.bind(2, doc.shortdesc);
        update.bind(3, doc.text);
        update.bind(4, hash);
        update.bind(5, id);
        update.step();
        return {id, true};
    }

    SqliteStatement insert(m_connection.handle(), QStringLiteral(
        "INSERT INTO documents (path, title, shortdesc, text, content_hash) VALUES (?, ?, ?, ?, ?)"));
    insert.bind(1, doc.path);
    insert.bind(2, doc.title);
    insert.bind(3, doc.shortdesc);
    insert.bind(4, doc.text);
    insert.bind(5, hash);
    insert.step();
    return {insert.lastInsertRowId(), true};
}

QVector<qint64> SqliteDocumentRepository::chunkIdsForDocument(qint64 documentId) {
    SqliteStatement stmt(m_connection.handle(), QStringLiteral("SELECT id FROM chunks WHERE doc_id = ?"));
    stmt.bind(1, documentId);

    QVector<qint64> ids;
    while (stmt.step()) {
        ids.push_back(stmt.columnInt64(0));
    }
    return ids;
}

QVector<Chunk> SqliteDocumentRepository::replaceChunks(qint64 documentId, const QVector<QString>& chunkTexts) {
    SqliteStatement remove(m_connection.handle(), QStringLiteral("DELETE FROM chunks WHERE doc_id = ?"));
    remove.bind(1, documentId);
    remove.step();

    SqliteStatement insert(m_connection.handle(),
                            QStringLiteral("INSERT INTO chunks (doc_id, chunk_index, text) VALUES (?, ?, ?)"));

    QVector<Chunk> chunks;
    chunks.reserve(chunkTexts.size());
    for (int i = 0; i < chunkTexts.size(); ++i) {
        insert.reset();
        insert.bind(1, documentId);
        insert.bind(2, static_cast<qint64>(i));
        insert.bind(3, chunkTexts.at(i));
        insert.step();

        Chunk chunk;
        chunk.id = insert.lastInsertRowId();
        chunk.documentId = documentId;
        chunk.chunkIndex = i;
        chunk.text = chunkTexts.at(i);
        chunks.push_back(chunk);
    }
    return chunks;
}

RetrievedChunks SqliteDocumentRepository::resolveChunks(const VectorMatches& matches) {
    SqliteStatement stmt(m_connection.handle(), QStringLiteral(
        "SELECT c.text, c.chunk_index, d.path, d.title "
        "FROM chunks c JOIN documents d ON d.id = c.doc_id WHERE c.id = ?"));

    RetrievedChunks result;
    result.reserve(matches.size());
    for (const auto& match : matches) {
        stmt.reset();
        stmt.bind(1, match.chunkId);
        if (!stmt.step()) {
            continue; // the chunk may have been deleted between search and resolution - skip it
        }

        RetrievedChunk chunk;
        chunk.text = stmt.columnText(0);
        chunk.chunkIndex = static_cast<int>(stmt.columnInt64(1));
        chunk.documentPath = stmt.columnText(2);
        chunk.documentTitle = stmt.columnText(3);
        chunk.distance = match.distance;
        result.push_back(chunk);
    }
    return result;
}

} // namespace winccmcp::infrastructure::persistence
