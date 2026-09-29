#ifndef WINCCMCP_CORE_INTERFACES_IJSONRPCHANDLER_H__B0C1D2E3_F4A0_4001_D8F3_EAFB0C1D2E3F__INCLUDED_
#define WINCCMCP_CORE_INTERFACES_IJSONRPCHANDLER_H__B0C1D2E3_F4A0_4001_D8F3_EAFB0C1D2E3F__INCLUDED_

#include "core/models/JsonRpc.h"

namespace winccmcp::core::interfaces {

// The contract for "what to do with one JSON-RPC request". Implemented by
// infrastructure::mcp::JsonRpcDispatcher (a Command table of method -> handler).
// The transport (IJsonRpcTransport) only knows this interface - the dispatcher
// doesn't care how the request reached the server (HTTP/stdio/whatever).
class IJsonRpcHandler {
public:
    virtual ~IJsonRpcHandler() = default;

    virtual models::JsonRpcResponse handle(const models::JsonRpcRequest& request) = 0;
};

} // namespace winccmcp::core::interfaces

#endif // WINCCMCP_CORE_INTERFACES_IJSONRPCHANDLER_H__B0C1D2E3_F4A0_4001_D8F3_EAFB0C1D2E3F__INCLUDED_
