#ifndef WINCCMCP_INFRASTRUCTURE_MCP_JSONRPCDISPATCHER_H__5A6B7C8D_9EEF_4E00_D899_EF3A4B5C6D7E__INCLUDED_
#define WINCCMCP_INFRASTRUCTURE_MCP_JSONRPCDISPATCHER_H__5A6B7C8D_9EEF_4E00_D899_EF3A4B5C6D7E__INCLUDED_

#include <functional>
#include <unordered_map>

#include "core/interfaces/IJsonRpcHandler.h"
#include "core/services/McpToolRegistry.h"

namespace winccmcp::infrastructure::mcp {

// A Command table of method -> handler for initialize/tools-list/tools-call.
// Knows nothing about HTTP - that's the job of IJsonRpcTransport (HttpJsonRpcTransport).
class JsonRpcDispatcher : public core::interfaces::IJsonRpcHandler {
public:
    explicit JsonRpcDispatcher(core::services::McpToolRegistry& toolRegistry);

    core::models::JsonRpcResponse handle(const core::models::JsonRpcRequest& request) override;

private:
    using MethodHandler = std::function<QJsonValue(const QJsonValue& params)>;

    core::services::McpToolRegistry& m_toolRegistry;
    std::unordered_map<std::string, MethodHandler> m_methods;

    void registerMethods();

    QJsonValue handleInitialize(const QJsonValue& params);
    QJsonValue handleToolsList(const QJsonValue& params);
    QJsonValue handleToolsCall(const QJsonValue& params);
};

} // namespace winccmcp::infrastructure::mcp

#endif // WINCCMCP_INFRASTRUCTURE_MCP_JSONRPCDISPATCHER_H__5A6B7C8D_9EEF_4E00_D899_EF3A4B5C6D7E__INCLUDED_
