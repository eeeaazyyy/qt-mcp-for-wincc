#ifndef WINCCMCP_INFRASTRUCTURE_PERSISTENCE_SQLITECHATHISTORYREPOSITORY_H__0B1C2D3E_4FAB_4BBC_8344_9FA0B1C2D3EA__INCLUDED_
#define WINCCMCP_INFRASTRUCTURE_PERSISTENCE_SQLITECHATHISTORYREPOSITORY_H__0B1C2D3E_4FAB_4BBC_8344_9FA0B1C2D3EA__INCLUDED_

#include "core/interfaces/IChatHistoryRepository.h"
#include "infrastructure/persistence/SqliteConnection.h"

namespace winccmcp::infrastructure::persistence {

class SqliteChatHistoryRepository : public core::interfaces::IChatHistoryRepository {
public:
    explicit SqliteChatHistoryRepository(SqliteConnection& connection);

    qint64 save(const core::models::ChatHistoryRecord& record) override;
    core::models::ChatHistoryRecords loadAll() override;

private:
    SqliteConnection& m_connection;
};

} // namespace winccmcp::infrastructure::persistence

#endif // WINCCMCP_INFRASTRUCTURE_PERSISTENCE_SQLITECHATHISTORYREPOSITORY_H__0B1C2D3E_4FAB_4BBC_8344_9FA0B1C2D3EA__INCLUDED_
