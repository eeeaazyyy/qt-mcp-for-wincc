#include "infrastructure/persistence/SqliteChatHistoryRepository.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "infrastructure/persistence/SqliteStatement.h"

namespace winccmcp::infrastructure::persistence {

using core::models::ChatHistoryRecord;
using core::models::ChatHistoryRecords;
using core::models::Source;
using core::models::Sources;

namespace {

QString sourcesToJson(const Sources& sources) {
    QJsonArray array;
    for (const auto& s : sources) {
        array.append(QJsonObject{{"path", s.path}, {"title", s.title}, {"distance", s.distance}});
    }
    return QString::fromUtf8(QJsonDocument(array).toJson(QJsonDocument::Compact));
}

Sources sourcesFromJson(const QString& json) {
    Sources sources;
    const QJsonArray array = QJsonDocument::fromJson(json.toUtf8()).array();
    for (const auto& value : array) {
        const QJsonObject o = value.toObject();
        sources.push_back(Source{o.value("path").toString(), o.value("title").toString(), o.value("distance").toDouble()});
    }
    return sources;
}

} // namespace

SqliteChatHistoryRepository::SqliteChatHistoryRepository(SqliteConnection& connection) : m_connection(connection) {
}

qint64 SqliteChatHistoryRepository::save(const ChatHistoryRecord& record) {
    SqliteStatement stmt(m_connection.handle(), QStringLiteral(
        "INSERT INTO chat_history (ts, query, answer, sources_json, duration_ms) VALUES (?, ?, ?, ?, ?)"));
    stmt.bind(1, record.timestamp.toString(Qt::ISODateWithMs));
    stmt.bind(2, record.query);
    stmt.bind(3, record.answer);
    stmt.bind(4, sourcesToJson(record.sources));
    stmt.bind(5, record.durationMs);
    stmt.step();
    return stmt.lastInsertRowId();
}

ChatHistoryRecords SqliteChatHistoryRepository::loadAll() {
    SqliteStatement stmt(m_connection.handle(), QStringLiteral(
        "SELECT id, ts, query, answer, sources_json, duration_ms FROM chat_history ORDER BY id"));

    ChatHistoryRecords records;
    while (stmt.step()) {
        ChatHistoryRecord record;
        record.id = stmt.columnInt64(0);
        record.timestamp = QDateTime::fromString(stmt.columnText(1), Qt::ISODateWithMs);
        record.query = stmt.columnText(2);
        record.answer = stmt.columnText(3);
        record.sources = sourcesFromJson(stmt.columnText(4));
        record.durationMs = stmt.columnInt64(5);
        records.push_back(record);
    }
    return records;
}

} // namespace winccmcp::infrastructure::persistence
