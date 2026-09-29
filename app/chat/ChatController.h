#ifndef WINCCMCP_APP_CHAT_CHATCONTROLLER_H__A5B6C7D8_E99A_4ABB_8234_D3E4F5A6B7C8__INCLUDED_
#define WINCCMCP_APP_CHAT_CHATCONTROLLER_H__A5B6C7D8_E99A_4ABB_8234_D3E4F5A6B7C8__INCLUDED_

#include <QElapsedTimer>
#include <QFutureWatcher>
#include <QObject>

#include "core/interfaces/IChatHistoryRepository.h"
#include "core/models/RagAnswer.h"
#include "core/services/RagService.h"

namespace winccmcp::app::chat {
class ChatView;
}

namespace winccmcp::app::history {
class SearchHistoryTableModel;
}

namespace winccmcp::app::chat {

// Controller in MVC: receives messageSubmitted from ChatView, calls
// core::services::RagService on a background thread (QtConcurrent::run +
// QFutureWatcher - no coroutines, the finished() signal delivers itself back
// to the GUI thread), updates the chat model, and at the same time writes a
// history entry (both to SQLite via IChatHistoryRepository, and to
// SearchHistoryTableModel).
class ChatController : public QObject {
    Q_OBJECT

public:
    ChatController(ChatView& view,
                   core::services::RagService& ragService,
                   core::interfaces::IChatHistoryRepository& historyRepository,
                   history::SearchHistoryTableModel& historyModel,
                   QObject* parent = nullptr);

private:
    void onMessageSubmitted(const QString& text);
    void onAnswerFinished();

    ChatView& m_view;
    core::services::RagService& m_ragService;
    core::interfaces::IChatHistoryRepository& m_historyRepository;
    history::SearchHistoryTableModel& m_historyModel;

    QString m_pendingQuestion;
    QElapsedTimer m_timer;
    QFutureWatcher<core::models::RagAnswer> m_watcher;
};

} // namespace winccmcp::app::chat

#endif // WINCCMCP_APP_CHAT_CHATCONTROLLER_H__A5B6C7D8_E99A_4ABB_8234_D3E4F5A6B7C8__INCLUDED_
