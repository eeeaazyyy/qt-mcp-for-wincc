#ifndef WINCCMCP_APP_CHAT_CHATMESSAGE_H__7C8D9EAF_B001_4022_F9AB_A4B5C6D7E8F9__INCLUDED_
#define WINCCMCP_APP_CHAT_CHATMESSAGE_H__7C8D9EAF_B001_4022_F9AB_A4B5C6D7E8F9__INCLUDED_

#include <QDateTime>
#include <QString>

namespace winccmcp::app::chat {

enum class ChatMessageKind { User, Assistant, SystemNotice };

// One entry in the chat feed (a Model-DTO for ChatMessageListModel). Kept
// separate from core::models::ChatTurn - a GUI message has a timestamp and a
// "system notice" kind (e.g. "index not built"), neither of which exist in
// the conversation sent to the LLM.
struct ChatMessage {
    ChatMessageKind kind = ChatMessageKind::User;
    QString text;
    QDateTime timestamp = QDateTime::currentDateTime();
};

} // namespace winccmcp::app::chat

#endif // WINCCMCP_APP_CHAT_CHATMESSAGE_H__7C8D9EAF_B001_4022_F9AB_A4B5C6D7E8F9__INCLUDED_
