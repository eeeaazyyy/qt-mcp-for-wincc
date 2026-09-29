#ifndef WINCCMCP_CORE_INTERFACES_IJSONRPCTRANSPORT_H__C1D2E3F4_A501_4112_E9A4_FB0C1D2E3F40__INCLUDED_
#define WINCCMCP_CORE_INTERFACES_IJSONRPCTRANSPORT_H__C1D2E3F4_A501_4112_E9A4_FB0C1D2E3F40__INCLUDED_

#include "core/interfaces/IJsonRpcHandler.h"

namespace winccmcp::core::interfaces {

// Transport kept separate from business logic (Adapter/Strategy): today this
// is infrastructure::mcp::HttpJsonRpcTransport (POST /mcp via cpp-httplib);
// tomorrow a stdio transport could be added without touching JsonRpcDispatcher
// or core::services.
class IJsonRpcTransport {
public:
    virtual ~IJsonRpcTransport() = default;

    virtual void start(IJsonRpcHandler& handler) = 0;
    virtual void stop() = 0;
};

} // namespace winccmcp::core::interfaces

#endif // WINCCMCP_CORE_INTERFACES_IJSONRPCTRANSPORT_H__C1D2E3F4_A501_4112_E9A4_FB0C1D2E3F40__INCLUDED_
