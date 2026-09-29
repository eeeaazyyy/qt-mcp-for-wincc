#ifndef WINCCMCP_CORE_MODELS_CHATHISTORYRECORD_H__6F708192_A314_4526_D748_F9A0B1C2D3E4__INCLUDED_
#define WINCCMCP_CORE_MODELS_CHATHISTORYRECORD_H__6F708192_A314_4526_D748_F9A0B1C2D3E4__INCLUDED_

#include <QDateTime>
#include <QString>
#include <QVector>

#include "core/models/Source.h"

namespace winccmcp::core::models {

// One entry in the "Search history" tab: one user question + its answer.
struct ChatHistoryRecord {
    qint64 id = 0;
    QDateTime timestamp;
    QString query;
    QString answer;
    Sources sources;
    qint64 durationMs = 0;
};

using ChatHistoryRecords = QVector<ChatHistoryRecord>;

} // namespace winccmcp::core::models

#endif // WINCCMCP_CORE_MODELS_CHATHISTORYRECORD_H__6F708192_A314_4526_D748_F9A0B1C2D3E4__INCLUDED_
