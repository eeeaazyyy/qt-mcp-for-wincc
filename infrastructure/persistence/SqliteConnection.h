#ifndef WINCCMCP_INFRASTRUCTURE_PERSISTENCE_SQLITECONNECTION_H__B6C7D8E9_FA56_4667_3EF9_4A5B6C7D8E95__INCLUDED_
#define WINCCMCP_INFRASTRUCTURE_PERSISTENCE_SQLITECONNECTION_H__B6C7D8E9_FA56_4667_3EF9_4A5B6C7D8E95__INCLUDED_

#include <QString>

struct sqlite3;

namespace winccmcp::infrastructure::persistence {

// RAII wrapper around a single sqlite3* connection. SqliteEnvironment::initializeOnce()
// must be called before the first SqliteConnection is created in the process.
class SqliteConnection {
public:
    explicit SqliteConnection(const QString& path);
    ~SqliteConnection();

    SqliteConnection(const SqliteConnection&) = delete;
    SqliteConnection& operator=(const SqliteConnection&) = delete;

    sqlite3* handle() const { return m_db; }

    // Executes SQL with no parameters and no result to read (DDL, PRAGMA).
    void exec(const QString& sql);

private:
    sqlite3* m_db = nullptr;
};

} // namespace winccmcp::infrastructure::persistence

#endif // WINCCMCP_INFRASTRUCTURE_PERSISTENCE_SQLITECONNECTION_H__B6C7D8E9_FA56_4667_3EF9_4A5B6C7D8E95__INCLUDED_
