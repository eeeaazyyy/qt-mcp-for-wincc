#include "infrastructure/mcp/JsonRpcDispatcher.h"

#include <stdexcept>

#include <QJsonArray>
#include <QJsonObject>

namespace winccmcp::infrastructure::mcp {

// JsonRpcErrorCode is a namespace with constants, not a class/enum, so a
// using-declaration (using X::JsonRpcErrorCode;) isn't valid for it - it
// needs a namespace alias instead.
namespace JsonRpcErrorCode = core::models::JsonRpcErrorCode;
using core::models::JsonRpcRequest;
using core::models::JsonRpcResponse;

JsonRpcDispatcher::JsonRpcDispatcher(core::services::McpToolRegistry& toolRegistry) : m_toolRegistry(toolRegistry) {
    registerMethods();
}

void JsonRpcDispatcher::registerMethods() {
    m_methods["initialize"] = [this](const QJsonValue& p) { return handleInitialize(p); };
    m_methods["tools/list"] = [this](const QJsonValue& p) { return handleToolsList(p); };
    m_methods["tools/call"] = [this](const QJsonValue& p) { return handleToolsCall(p); };
}

JsonRpcResponse JsonRpcDispatcher::handle(const JsonRpcRequest& request) {
    const auto it = m_methods.find(request.method.toStdString());
    if (it == m_methods.end()) {
        return JsonRpcResponse::failure(request.id, JsonRpcErrorCode::MethodNotFound,
                                         QStringLiteral("Method not found: %1").arg(request.method));
    }

    try {
        return JsonRpcResponse::success(request.id, it->second(request.params));
    } catch (const std::invalid_argument& e) {
        return JsonRpcResponse::failure(request.id, JsonRpcErrorCode::InvalidParams, QString::fromUtf8(e.what()));
    } catch (const std::exception& e) {
        return JsonRpcResponse::failure(request.id, JsonRpcErrorCode::InternalError, QString::fromUtf8(e.what()));
    }
}

QJsonValue JsonRpcDispatcher::handleInitialize(const QJsonValue& /*params*/) {
    return QJsonObject{
        {"protocolVersion", "2025-06-18"},
        {"serverInfo", QJsonObject{{"name", "qt-mcp-for-wincc"}, {"version", "0.1.0"}}},
        {"capabilities", QJsonObject{{"tools", QJsonObject{}}}},
    };
}

QJsonValue JsonRpcDispatcher::handleToolsList(const QJsonValue& /*params*/) {
    QJsonArray tools;
    for (const auto& tool : m_toolRegistry.listTools()) {
        tools.append(QJsonObject{
            {"name", tool.name},
            {"description", tool.description},
            {"inputSchema", tool.inputSchema},
        });
    }
    return QJsonObject{{"tools", tools}};
}

QJsonValue JsonRpcDispatcher::handleToolsCall(const QJsonValue& params) {
    const QJsonObject obj = params.toObject();
    const QString name = obj.value("name").toString();
    if (name.isEmpty()) {
        throw std::invalid_argument("tools/call: parameter 'name' is required");
    }
    return m_toolRegistry.callTool(name, obj.value("arguments"));
}

} // namespace winccmcp::infrastructure::mcp
