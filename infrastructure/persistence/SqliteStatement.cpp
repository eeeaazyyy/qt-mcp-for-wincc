#include "infrastructure/persistence/SqliteStatement.h"

#include <stdexcept>

#include <sqlite3.h>

namespace winccmcp::infrastructure::persistence {

SqliteStatement::SqliteStatement(sqlite3* db, const QString& sql) : m_db(db) {
    const QByteArray sqlUtf8 = sql.toUtf8();
    if (sqlite3_prepare_v2(m_db, sqlUtf8.constData(), sqlUtf8.size() + 1, &m_stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("SQLite prepare failed: " + QString::fromUtf8(sqlite3_errmsg(m_db)).toStdString()
                                  + " -- SQL: " + sql.toStdString());
    }
}

SqliteStatement::~SqliteStatement() {
    if (m_stmt) {
        sqlite3_finalize(m_stmt);
    }
}

void SqliteStatement::bind(int index, qint64 value) {
    sqlite3_bind_int64(m_stmt, index, value);
}

void SqliteStatement::bind(int index, double value) {
    sqlite3_bind_double(m_stmt, index, value);
}

void SqliteStatement::bind(int index, const QString& value) {
    const QByteArray utf8 = value.toUtf8();
    sqlite3_bind_text(m_stmt, index, utf8.constData(), utf8.size(), SQLITE_TRANSIENT);
}

void SqliteStatement::bindBlob(int index, const void* data, int sizeBytes) {
    sqlite3_bind_blob(m_stmt, index, data, sizeBytes, SQLITE_TRANSIENT);
}

void SqliteStatement::bindNull(int index) {
    sqlite3_bind_null(m_stmt, index);
}

bool SqliteStatement::step() {
    const int rc = sqlite3_step(m_stmt);
    if (rc == SQLITE_ROW) {
        return true;
    }
    if (rc == SQLITE_DONE) {
        return false;
    }
    throw std::runtime_error("SQLite step failed: " + QString::fromUtf8(sqlite3_errmsg(m_db)).toStdString());
}

void SqliteStatement::reset() {
    sqlite3_reset(m_stmt);
    sqlite3_clear_bindings(m_stmt);
}

qint64 SqliteStatement::columnInt64(int index) const {
    return sqlite3_column_int64(m_stmt, index);
}

double SqliteStatement::columnDouble(int index) const {
    return sqlite3_column_double(m_stmt, index);
}

QString SqliteStatement::columnText(int index) const {
    const unsigned char* text = sqlite3_column_text(m_stmt, index);
    const int bytes = sqlite3_column_bytes(m_stmt, index);
    return QString::fromUtf8(reinterpret_cast<const char*>(text), bytes);
}

qint64 SqliteStatement::lastInsertRowId() const {
    return sqlite3_last_insert_rowid(m_db);
}

} // namespace winccmcp::infrastructure::persistence
