#ifndef WINCCMCP_CORE_MODELS_JSONRPC_H__708192A3_B415_4637_E859_A0B1C2D3E4F5__INCLUDED_
#define WINCCMCP_CORE_MODELS_JSONRPC_H__708192A3_B415_4637_E859_A0B1C2D3E4F5__INCLUDED_

#include <optional>

#include <QJsonObject>
#include <QJsonValue>
#include <QString>

// Models for the JSON-RPC 2.0 / MCP-like protocol (initialize, tools/list,
// tools/call). They use QJsonValue as a plain value-type from Qt::Core -
// no GUI, no event loop - so this doesn't break "core with almost no Qt".

namespace winccmcp::core::models {

struct JsonRpcError {
    int code = 0;
    QString message;
    QJsonValue data;
};

struct JsonRpcRequest {
    QString method;
    QJsonValue params;   // an object or array of method parameters
    QJsonValue id;       // number/string; isUndefined() == a notification with no reply

    bool isNotification() const { return id.isUndefined(); }
};

struct JsonRpcResponse {
    QJsonValue id;
    QJsonValue result;                    // valid if error isn't set
    std::optional<JsonRpcError> error;

    bool isError() const { return error.has_value(); }

    static JsonRpcResponse success(const QJsonValue& id, const QJsonValue& result) {
        JsonRpcResponse r;
        r.id = id;
        r.result = result;
        return r;
    }

    static JsonRpcResponse failure(const QJsonValue& id, int code, const QString& message) {
        JsonRpcResponse r;
        r.id = id;
        r.error = JsonRpcError{code, message, QJsonValue()};
        return r;
    }
};

// Standard JSON-RPC 2.0 error codes.
namespace JsonRpcErrorCode {
constexpr int ParseError = -32700;
constexpr int InvalidRequest = -32600;
constexpr int MethodNotFound = -32601;
constexpr int InvalidParams = -32602;
constexpr int InternalError = -32603;
} // namespace JsonRpcErrorCode

// Description of one MCP tool for the tools/list response.
struct ToolDefinition {
    QString name;
    QString description;
    QJsonObject inputSchema;
};

} // namespace winccmcp::core::models

#endif // WINCCMCP_CORE_MODELS_JSONRPC_H__708192A3_B415_4637_E859_A0B1C2D3E4F5__INCLUDED_
