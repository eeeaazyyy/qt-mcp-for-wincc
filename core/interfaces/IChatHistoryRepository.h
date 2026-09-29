#ifndef WINCCMCP_CORE_INTERFACES_ICHATHISTORYREPOSITORY_H__E7F8A9B0_C17D_4D9E_A5C0_B7C8D9EAFB0C__INCLUDED_
#define WINCCMCP_CORE_INTERFACES_ICHATHISTORYREPOSITORY_H__E7F8A9B0_C17D_4D9E_A5C0_B7C8D9EAFB0C__INCLUDED_

#include "core/models/ChatHistoryRecord.h"

namespace winccmcp::core::interfaces {

// Persistence for the "Search history" tab - kept separate from
// IDocumentRepository (ISP): documentation and chat history change and are
// read for different reasons.
class IChatHistoryRepository {
public:
    virtual ~IChatHistoryRepository() = default;

    // record.id is ignored on input; the implementation assigns the real id
    // and returns it.
    virtual qint64 save(const models::ChatHistoryRecord& record) = 0;

    // The entire history, ordered by time - used to populate
    // SearchHistoryTableModel on application startup.
    virtual models::ChatHistoryRecords loadAll() = 0;
};

} // namespace winccmcp::core::interfaces

#endif // WINCCMCP_CORE_INTERFACES_ICHATHISTORYREPOSITORY_H__E7F8A9B0_C17D_4D9E_A5C0_B7C8D9EAFB0C__INCLUDED_
