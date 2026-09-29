#ifndef WINCCMCP_CORE_INTERFACES_ILLMCHATCLIENT_H__A3B4C5D6_E748_495A_C18C_D3E4F5A6B7C8__INCLUDED_
#define WINCCMCP_CORE_INTERFACES_ILLMCHATCLIENT_H__A3B4C5D6_E748_495A_C18C_D3E4F5A6B7C8__INCLUDED_

#include <QString>

#include "core/models/ChatTurn.h"

namespace winccmcp::core::interfaces {

// Abstraction over text generation by the local LLM (adapter - LmStudioClient
// in infrastructure/llm). Through this interface, core::services::RagService
// knows nothing about LM Studio or HTTP.
class ILlmChatClient {
public:
    virtual ~ILlmChatClient() = default;

    virtual QString chat(const models::ChatTurns& messages) = 0;
};

} // namespace winccmcp::core::interfaces

#endif // WINCCMCP_CORE_INTERFACES_ILLMCHATCLIENT_H__A3B4C5D6_E748_495A_C18C_D3E4F5A6B7C8__INCLUDED_
