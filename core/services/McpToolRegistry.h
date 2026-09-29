#ifndef WINCCMCP_CORE_SERVICES_MCPTOOLREGISTRY_H__F4A5B6C7_D834_4445_1CD7_2E3F4A5B6C73__INCLUDED_
#define WINCCMCP_CORE_SERVICES_MCPTOOLREGISTRY_H__F4A5B6C7_D834_4445_1CD7_2E3F4A5B6C73__INCLUDED_

#include <functional>
#include <unordered_map>

#include <QJsonValue>
#include <QString>
#include <QVector>

#include "core/interfaces/IDocumentRepository.h"
#include "core/models/JsonRpc.h"
#include "core/services/RagService.h"

namespace winccmcp::core::services {

// A Command registry of tools exposed over JSON-RPC (tools/list, tools/call):
// search_docs, ask_docs, get_document, list_documents - the same 4 tools as
// in the Python version of the project. Knows nothing about HTTP/transport
// itself - that's the job of infrastructure::mcp::JsonRpcDispatcher.
class McpToolRegistry {
public:
    McpToolRegistry(RagService& ragService, interfaces::IDocumentRepository& documentRepository);

    QVector<models::ToolDefinition> listTools() const;

    // Not const: RagService/IDocumentRepository talk to the network and
    // SQLite, so there's no meaningful const invariant worth faking here.
    // Throws std::invalid_argument if the tool isn't found or the parameters
    // are invalid - JsonRpcDispatcher turns this into a JSON-RPC InvalidParams.
    QJsonValue callTool(const QString& name, const QJsonValue& arguments);

private:
    using ToolHandler = std::function<QJsonValue(const QJsonValue&)>;

    RagService& m_ragService;
    interfaces::IDocumentRepository& m_documentRepository;
    std::unordered_map<std::string, ToolHandler> m_handlers;
    QVector<models::ToolDefinition> m_definitions;

    void registerTools();

    QJsonValue searchDocs(const QJsonValue& arguments);
    QJsonValue askDocs(const QJsonValue& arguments);
    QJsonValue getDocument(const QJsonValue& arguments);
    QJsonValue listDocuments(const QJsonValue& arguments);
};

} // namespace winccmcp::core::services

#endif // WINCCMCP_CORE_SERVICES_MCPTOOLREGISTRY_H__F4A5B6C7_D834_4445_1CD7_2E3F4A5B6C73__INCLUDED_
