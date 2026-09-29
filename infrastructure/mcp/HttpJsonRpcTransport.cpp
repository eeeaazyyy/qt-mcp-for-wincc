#include "infrastructure/mcp/HttpJsonRpcTransport.h"

#include <utility>

#include <QJsonDocument>
#include <QJsonObject>

#include <httplib.h>

namespace winccmcp::infrastructure::mcp {

using core::interfaces::IJsonRpcHandler;
// JsonRpcErrorCode is a namespace, not a class/enum: a using-declaration
// isn't valid for it, so a namespace alias is used instead (same trick as in
// JsonRpcDispatcher.cpp).
namespace JsonRpcErrorCode = core::models::JsonRpcErrorCode;
using core::models::JsonRpcRequest;
using core::models::JsonRpcResponse;

namespace {

JsonRpcRequest parseRequest(const QJsonObject& obj) {
    JsonRpcRequest request;
    request.method = obj.value("method").toString();
    request.params = obj.value("params");
    request.id = obj.value("id"); // Undefined if the key is absent - means a notification
    return request;
}

QJsonObject serializeResponse(const JsonRpcResponse& response) {
    QJsonObject obj{{"jsonrpc", "2.0"}, {"id", response.id}};
    if (response.isError()) {
        obj["error"] = QJsonObject{{"code", response.error->code}, {"message", response.error->message}};
    } else {
        obj["result"] = response.result;
    }
    return obj;
}

} // namespace

HttpJsonRpcTransport::HttpJsonRpcTransport(QString host, int port) : m_host(std::move(host)), m_port(port) {
}

HttpJsonRpcTransport::~HttpJsonRpcTransport() {
    stop();
}

void HttpJsonRpcTransport::start(IJsonRpcHandler& handler) {
    m_server = std::make_unique<httplib::Server>();

    m_server->Post("/mcp", [&handler](const httplib::Request& req, httplib::Response& res) {
        const QJsonDocument requestDoc = QJsonDocument::fromJson(QByteArray::fromStdString(req.body));

        JsonRpcResponse response;
        if (!requestDoc.isObject()) {
            response = JsonRpcResponse::failure(QJsonValue(), JsonRpcErrorCode::ParseError, "Invalid JSON in the request body");
        } else {
            response = handler.handle(parseRequest(requestDoc.object()));
        }

        const QByteArray body = QJsonDocument(serializeResponse(response)).toJson(QJsonDocument::Compact);
        res.set_content(body.toStdString(), "application/json");
    });

    const QByteArray hostUtf8 = m_host.toUtf8();
    m_thread = std::jthread([this, hostUtf8](const std::stop_token&) {
        m_server->listen(hostUtf8.constData(), m_port);
    });
}

void HttpJsonRpcTransport::stop() {
    if (m_server) {
        m_server->stop();
    }
    if (m_thread.joinable()) {
        m_thread.join();
    }
    m_server.reset();
}

} // namespace winccmcp::infrastructure::mcp
