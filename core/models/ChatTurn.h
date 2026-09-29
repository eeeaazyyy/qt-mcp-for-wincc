#ifndef WINCCMCP_CORE_MODELS_CHATTURN_H__92A3B4C5_D637_4859_B07B_C2D3E4F5A6B7__INCLUDED_
#define WINCCMCP_CORE_MODELS_CHATTURN_H__92A3B4C5_D637_4859_B07B_C2D3E4F5A6B7__INCLUDED_

#include <QString>
#include <QVector>

namespace winccmcp::core::models {

enum class ChatRole { System, User, Assistant };

// One turn in the conversation sent to ILlmChatClient::chat().
struct ChatTurn {
    ChatRole role = ChatRole::User;
    QString content;
};

using ChatTurns = QVector<ChatTurn>;

} // namespace winccmcp::core::models

#endif // WINCCMCP_CORE_MODELS_CHATTURN_H__92A3B4C5_D637_4859_B07B_C2D3E4F5A6B7__INCLUDED_
