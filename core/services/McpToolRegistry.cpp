#include "core/services/McpToolRegistry.h"

#include <stdexcept>

#include <QJsonArray>
#include <QJsonObject>

namespace winccmcp::core::services {

namespace {

QString stringArg(const QJsonObject& args, const QString& key, const QString& defaultValue = QString()) {
    return args.contains(key) ? args.value(key).toString() : defaultValue;
}

int intArg(const QJsonObject& args, const QString& key, int defaultValue) {
    return args.contains(key) ? args.value(key).toInt(defaultValue) : defaultValue;
}

QJsonObject sourceToJson(const models::Source& s) {
    QJsonObject o;
    o["path"] = s.path;
    o["title"] = s.title;
    o["distance"] = s.distance;
    return o;
}

QJsonObject documentToJson(const models::Document& d) {
    QJsonObject o;
    o["path"] = d.path;
    o["title"] = d.title;
    o["shortdesc"] = d.shortdesc;
    return o;
}

} // namespace

McpToolRegistry::McpToolRegistry(RagService& ragService, interfaces::IDocumentRepository& documentRepository)
    : m_ragService(ragService)
    , m_documentRepository(documentRepository) {
    registerTools();
}

void McpToolRegistry::registerTools() {
    m_handlers["search_docs"] = [this](const QJsonValue& a) { return searchDocs(a); };
    m_handlers["ask_docs"] = [this](const QJsonValue& a) { return askDocs(a); };
    m_handlers["get_document"] = [this](const QJsonValue& a) { return getDocument(a); };
    m_handlers["list_documents"] = [this](const QJsonValue& a) { return listDocuments(a); };

    m_definitions = {
        models::ToolDefinition{
            "search_docs",
            "Semantic search over the WinCC OA documentation without calling the LLM - "
            "returns raw fragments and sources.",
            QJsonObject{
                {"type", "object"},
                {"properties", QJsonObject{
                    {"query", QJsonObject{{"type", "string"}}},
                    {"top_k", QJsonObject{{"type", "integer"}, {"default", 5}}},
                }},
                {"required", QJsonArray{"query"}},
            }},
        models::ToolDefinition{
            "ask_docs",
            "A question about the WinCC OA documentation, answered by the local LLM (RAG).",
            QJsonObject{
                {"type", "object"},
                {"properties", QJsonObject{
                    {"question", QJsonObject{{"type", "string"}}},
                    {"top_k", QJsonObject{{"type", "integer"}, {"default", 5}}},
                }},
                {"required", QJsonArray{"question"}},
            }},
        models::ToolDefinition{
            "get_document",
            "Full text of a single documentation page by its relative path.",
            QJsonObject{
                {"type", "object"},
                {"properties", QJsonObject{
                    {"path", QJsonObject{{"type", "string"}}},
                }},
                {"required", QJsonArray{"path"}},
            }},
        models::ToolDefinition{
            "list_documents",
            "List of indexed pages, filtered by a substring in title/path.",
            QJsonObject{
                {"type", "object"},
                {"properties", QJsonObject{
                    {"prefix", QJsonObject{{"type", "string"}, {"default", ""}}},
                    {"limit", QJsonObject{{"type", "integer"}, {"default", 50}}},
                }},
            }},
    };
}

QVector<models::ToolDefinition> McpToolRegistry::listTools() const {
    return m_definitions;
}

QJsonValue McpToolRegistry::callTool(const QString& name, const QJsonValue& arguments) {
    const auto it = m_handlers.find(name.toStdString());
    if (it == m_handlers.end()) {
        throw std::invalid_argument("Unknown tool: " + name.toStdString());
    }
    return it->second(arguments);
}

QJsonValue McpToolRegistry::searchDocs(const QJsonValue& arguments) {
    const QJsonObject args = arguments.toObject();
    const QString query = stringArg(args, "query");
    if (query.isEmpty()) {
        throw std::invalid_argument("search_docs: parameter 'query' is required");
    }
    const int topK = intArg(args, "top_k", 5);

    const auto chunks = m_ragService.retrieve(query, topK);

    QJsonArray result;
    for (const auto& c : chunks) {
        QJsonObject o;
        o["path"] = c.documentPath;
        o["title"] = c.documentTitle;
        o["chunk_index"] = c.chunkIndex;
        o["text"] = c.text;
        o["distance"] = c.distance;
        result.append(o);
    }
    return result;
}

QJsonValue McpToolRegistry::askDocs(const QJsonValue& arguments) {
    const QJsonObject args = arguments.toObject();
    const QString question = stringArg(args, "question");
    if (question.isEmpty()) {
        throw std::invalid_argument("ask_docs: parameter 'question' is required");
    }
    const int topK = intArg(args, "top_k", 5);

    const auto response = m_ragService.answer(question, topK);

    QJsonArray sourcesJson;
    for (const auto& s : response.sources) {
        sourcesJson.append(sourceToJson(s));
    }

    QJsonObject result;
    result["answer"] = response.answer;
    result["sources"] = sourcesJson;
    return result;
}

QJsonValue McpToolRegistry::getDocument(const QJsonValue& arguments) {
    const QJsonObject args = arguments.toObject();
    const QString path = stringArg(args, "path");
    if (path.isEmpty()) {
        throw std::invalid_argument("get_document: parameter 'path' is required");
    }

    const auto doc = m_documentRepository.findByPath(path);
    if (!doc) {
        QJsonObject error;
        error["error"] = QStringLiteral("No document with path '%1' found in the index.").arg(path);
        return error;
    }

    QJsonObject result = documentToJson(*doc);
    result["text"] = doc->text;
    return result;
}

QJsonValue McpToolRegistry::listDocuments(const QJsonValue& arguments) {
    const QJsonObject args = arguments.toObject();
    const QString prefix = stringArg(args, "prefix");
    const int limit = intArg(args, "limit", 50);

    const auto docs = m_documentRepository.search(prefix, limit);

    QJsonArray result;
    for (const auto& d : docs) {
        result.append(documentToJson(d));
    }
    return result;
}

} // namespace winccmcp::core::services
