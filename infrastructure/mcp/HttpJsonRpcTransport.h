#ifndef WINCCMCP_INFRASTRUCTURE_MCP_HTTPJSONRPCTRANSPORT_H__6B7C8D9E_AFF0_4F11_E90A_F3A4B5C6D7E8__INCLUDED_
#define WINCCMCP_INFRASTRUCTURE_MCP_HTTPJSONRPCTRANSPORT_H__6B7C8D9E_AFF0_4F11_E90A_F3A4B5C6D7E8__INCLUDED_

#include <memory>
#include <thread>

#include <QString>

#include "core/interfaces/IJsonRpcTransport.h"

namespace httplib {
class Server;
} // namespace httplib

namespace winccmcp::infrastructure::mcp {

// Adapter: httplib::Server -> IJsonRpcTransport. Implements a stateless
// variant of the MCP Streamable HTTP transport - one JSON-RPC request in the
// POST /mcp body, one JSON-RPC response in the response body, no SSE stream
// or sessions. This is a learning demonstration of "transport kept separate
// from business logic", not a channel for connecting external MCP clients
// (see README).
//
// httplib::Server::listen() is blocking, so it runs on a dedicated
// std::jthread; jthread's RAII destructor joins the thread itself once
// stop() has stopped listen().
class HttpJsonRpcTransport : public core::interfaces::IJsonRpcTransport {
public:
    HttpJsonRpcTransport(QString host, int port);
    ~HttpJsonRpcTransport() override;

    HttpJsonRpcTransport(const HttpJsonRpcTransport&) = delete;
    HttpJsonRpcTransport& operator=(const HttpJsonRpcTransport&) = delete;

    void start(core::interfaces::IJsonRpcHandler& handler) override;
    void stop() override;

private:
    QString m_host;
    int m_port;
    std::unique_ptr<httplib::Server> m_server;
    std::jthread m_thread;
};

} // namespace winccmcp::infrastructure::mcp

#endif // WINCCMCP_INFRASTRUCTURE_MCP_HTTPJSONRPCTRANSPORT_H__6B7C8D9E_AFF0_4F11_E90A_F3A4B5C6D7E8__INCLUDED_
