#include "app/chat/ChatController.h"

#include <exception>

#include <QtConcurrentRun>

#include "app/chat/ChatMessage.h"
#include "app/chat/ChatMessageListModel.h"
#include "app/chat/ChatView.h"
#include "app/common/MessagedException.h"
#include "app/history/SearchHistoryTableModel.h"

namespace winccmcp::app::chat {

using core::models::ChatHistoryRecord;
using core::models::RagAnswer;

ChatController::ChatController(ChatView& view,
                                core::services::RagService& ragService,
                                core::interfaces::IChatHistoryRepository& historyRepository,
                                history::SearchHistoryTableModel& historyModel,
                                QObject* parent)
    : QObject(parent)
    , m_view(view)
    , m_ragService(ragService)
    , m_historyRepository(historyRepository)
    , m_historyModel(historyModel) {
    connect(&m_view, &ChatView::messageSubmitted, this, &ChatController::onMessageSubmitted);
    connect(&m_watcher, &QFutureWatcher<RagAnswer>::finished, this, &ChatController::onAnswerFinished);
}

void ChatController::onMessageSubmitted(const QString& text) {
    m_view.model()->append(ChatMessage{ChatMessageKind::User, text});
    m_view.setInputEnabled(false);

    m_pendingQuestion = text;
    m_timer.start();

    m_watcher.setFuture(QtConcurrent::run([this, text]() -> RagAnswer {
        try {
            return m_ragService.answer(text, -1);
        } catch (const std::exception& e) {
            // Wrap it in a QException subclass, otherwise QtConcurrent would
            // lose the error message (see the comment in MessagedException.h).
            throw common::MessagedException(QString::fromUtf8(e.what()));
        }
    }));
}

void ChatController::onAnswerFinished() {
    RagAnswer response;
    try {
        response = m_watcher.result();
    } catch (const std::exception& e) {
        m_view.model()->append(
            ChatMessage{ChatMessageKind::SystemNotice, tr("Error while contacting LM Studio: %1").arg(QString::fromUtf8(e.what()))});
        m_view.setInputEnabled(true);
        return;
    }

    const qint64 elapsedMs = m_timer.elapsed();

    m_view.model()->append(ChatMessage{ChatMessageKind::Assistant, response.answer});
    m_view.setInputEnabled(true);

    ChatHistoryRecord record;
    record.timestamp = QDateTime::currentDateTime();
    record.query = m_pendingQuestion;
    record.answer = response.answer;
    record.sources = response.sources;
    record.durationMs = elapsedMs;
    record.id = m_historyRepository.save(record);

    m_historyModel.addRecord(record);
}

} // namespace winccmcp::app::chat
