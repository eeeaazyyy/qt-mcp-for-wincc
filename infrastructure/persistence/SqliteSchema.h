#ifndef WINCCMCP_INFRASTRUCTURE_PERSISTENCE_SQLITESCHEMA_H__D8E9FA0B_1C78_4889_5011_6C7D8E9FA0B7__INCLUDED_
#define WINCCMCP_INFRASTRUCTURE_PERSISTENCE_SQLITESCHEMA_H__D8E9FA0B_1C78_4889_5011_6C7D8E9FA0B7__INCLUDED_

#include "infrastructure/persistence/SqliteConnection.h"

namespace winccmcp::infrastructure::persistence {

// A single point for creating tables (documents/chunks/vec_chunks/chat_history) -
// SqliteVectorStore, SqliteDocumentRepository and SqliteChatHistoryRepository
// operate on top of an already-ready schema instead of creating pieces of it
// one after another.
class SqliteSchema {
public:
    // Throws std::runtime_error if the database was already created with a
    // different embedding dimension (the model in LM Studio changed) - in
    // that case the database file must be deleted and the documentation
    // re-indexed.
    static void ensureCreated(SqliteConnection& connection, int embeddingDim);
};

} // namespace winccmcp::infrastructure::persistence

#endif // WINCCMCP_INFRASTRUCTURE_PERSISTENCE_SQLITESCHEMA_H__D8E9FA0B_1C78_4889_5011_6C7D8E9FA0B7__INCLUDED_
