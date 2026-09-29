#ifndef WINCCMCP_INFRASTRUCTURE_PERSISTENCE_SQLITEENVIRONMENT_H__A5B6C7D8_E945_4556_2DE8_3F4A5B6C7D84__INCLUDED_
#define WINCCMCP_INFRASTRUCTURE_PERSISTENCE_SQLITEENVIRONMENT_H__A5B6C7D8_E945_4556_2DE8_3F4A5B6C7D84__INCLUDED_

namespace winccmcp::infrastructure::persistence {

// One-time (per-process) sqlite3 setup: registers sqlite-vec as a "built-in"
// extension via sqlite3_auto_extension, so every new sqlite3_open()
// automatically gets vec0 table support - without dynamically loading an
// external .dll.
//
// Called once from the composition root (app_main/main.cpp) before opening
// any connections.
class SqliteEnvironment {
public:
    static void initializeOnce();
};

} // namespace winccmcp::infrastructure::persistence

#endif // WINCCMCP_INFRASTRUCTURE_PERSISTENCE_SQLITEENVIRONMENT_H__A5B6C7D8_E945_4556_2DE8_3F4A5B6C7D84__INCLUDED_
