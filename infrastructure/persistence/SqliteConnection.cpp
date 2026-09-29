#include "infrastructure/persistence/SqliteConnection.h"

#include <stdexcept>

#include <sqlite3.h>

namespace winccmcp::infrastructure::persistence {

SqliteConnection::SqliteConnection(const QString& path) {
    const QByteArray pathUtf8 = path.toUtf8();
    if (sqlite3_open(pathUtf8.constData(), &m_db) != SQLITE_OK) {
        const QString message = QString::fromUtf8(sqlite3_errmsg(m_db));
        sqlite3_close(m_db);
        throw std::runtime_error("Failed to open SQLite database '" + path.toStdString() + "': " + message.toStdString());
    }
    exec(QStringLiteral("PRAGMA foreign_keys = ON"));
}

SqliteConnection::~SqliteConnection() {
    if (m_db) {
        sqlite3_close(m_db);
    }
}

void SqliteConnection::exec(const QString& sql) {
    char* errorMessage = nullptr;
    const QByteArray sqlUtf8 = sql.toUtf8();
    if (sqlite3_exec(m_db, sqlUtf8.constData(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        const QString message = errorMessage ? QString::fromUtf8(errorMessage) : QStringLiteral("unknown error");
        sqlite3_free(errorMessage);
        throw std::runtime_error("SQLite exec failed: " + message.toStdString());
    }
}

} // namespace winccmcp::infrastructure::persistence
