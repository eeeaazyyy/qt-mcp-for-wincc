#ifndef WINCCMCP_INFRASTRUCTURE_PERSISTENCE_SQLITESTATEMENT_H__C7D8E9FA_0B67_4778_4F00_5B6C7D8E9FA6__INCLUDED_
#define WINCCMCP_INFRASTRUCTURE_PERSISTENCE_SQLITESTATEMENT_H__C7D8E9FA_0B67_4778_4F00_5B6C7D8E9FA6__INCLUDED_

#include <QByteArray>
#include <QString>

struct sqlite3;
struct sqlite3_stmt;

namespace winccmcp::infrastructure::persistence {

// RAII wrapper around sqlite3_stmt* with a typed bind/column API. Not
// thread-safe and not copyable - one statement for one sequential call to
// bind*() -> step() -> column*()/reset().
class SqliteStatement {
public:
    SqliteStatement(sqlite3* db, const QString& sql);
    ~SqliteStatement();

    SqliteStatement(const SqliteStatement&) = delete;
    SqliteStatement& operator=(const SqliteStatement&) = delete;

    void bind(int index, qint64 value);
    void bind(int index, double value);
    void bind(int index, const QString& value);
    void bindBlob(int index, const void* data, int sizeBytes);
    void bindNull(int index);

    // true if a result row is available (SQLITE_ROW); false on SQLITE_DONE.
    bool step();
    void reset();

    qint64 columnInt64(int index) const;
    double columnDouble(int index) const;
    QString columnText(int index) const;

    qint64 lastInsertRowId() const;

private:
    sqlite3* m_db = nullptr;
    sqlite3_stmt* m_stmt = nullptr;
};

} // namespace winccmcp::infrastructure::persistence

#endif // WINCCMCP_INFRASTRUCTURE_PERSISTENCE_SQLITESTATEMENT_H__C7D8E9FA_0B67_4778_4F00_5B6C7D8E9FA6__INCLUDED_
