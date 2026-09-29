#include <gtest/gtest.h>

#include <memory>

#include <QJsonArray>
#include <QJsonObject>

#include "core/services/McpToolRegistry.h"
#include "core/services/RagService.h"
#include "infrastructure/mcp/JsonRpcDispatcher.h"
#include "tests/TestFakes.h"

using namespace winccmcp::core;
using namespace winccmcp::infrastructure::mcp;
using namespace winccmcp::tests;

namespace {

class JsonRpcDispatcherTest : public ::testing::Test {
protected:
    FakeEmbeddingClient embedding;
    FakeChatClient chat;
    FakeVectorStore vectorStore;
    FakeDocumentRepository documentRepository;
    std::unique_ptr<services::RagService> ragService;
    std::unique_ptr<services::McpToolRegistry> toolRegistry;
    std::unique_ptr<JsonRpcDispatcher> dispatcher;

    void SetUp() override {
        ragService = std::make_unique<services::RagService>(embedding, chat, vectorStore, documentRepository);
        toolRegistry = std::make_unique<services::McpToolRegistry>(*ragService, documentRepository);
        dispatcher = std::make_unique<JsonRpcDispatcher>(*toolRegistry);
    }

    models::JsonRpcRequest makeRequest(const QString& method, const QJsonValue& params, int id = 1) {
        models::JsonRpcRequest request;
        request.method = method;
        request.params = params;
        request.id = id;
        return request;
    }
};

TEST_F(JsonRpcDispatcherTest, ToolsListReturnsFourTools) {
    const auto response = dispatcher->handle(makeRequest("tools/list", QJsonObject{}));
    ASSERT_FALSE(response.isError());

    const QJsonArray tools = response.result.toObject().value("tools").toArray();
    EXPECT_EQ(tools.size(), 4);
}

TEST_F(JsonRpcDispatcherTest, UnknownMethodReturnsMethodNotFound) {
    const auto response = dispatcher->handle(makeRequest("no/such/method", QJsonObject{}));
    ASSERT_TRUE(response.isError());
    EXPECT_EQ(response.error->code, models::JsonRpcErrorCode::MethodNotFound);
}

TEST_F(JsonRpcDispatcherTest, ToolsCallWithoutNameReturnsInvalidParams) {
    const auto response = dispatcher->handle(makeRequest("tools/call", QJsonObject{{"arguments", QJsonObject{}}}));
    ASSERT_TRUE(response.isError());
    EXPECT_EQ(response.error->code, models::JsonRpcErrorCode::InvalidParams);
}

TEST_F(JsonRpcDispatcherTest, ToolsCallListDocumentsReturnsUpsertedDocument) {
    models::Document doc;
    doc.path = "ControlA_D/dpGet.html";
    doc.title = "dpGet()";
    documentRepository.documentsByPath[doc.path] = doc;

    const QJsonObject params{{"name", "list_documents"}, {"arguments", QJsonObject{{"prefix", "dpGet"}}}};
    const auto response = dispatcher->handle(makeRequest("tools/call", params));

    ASSERT_FALSE(response.isError());
    const QJsonArray docs = response.result.toArray();
    ASSERT_EQ(docs.size(), 1);
    EXPECT_EQ(docs.first().toObject().value("path").toString(), doc.path);
}

TEST_F(JsonRpcDispatcherTest, ToolsCallGetDocumentReturnsErrorForUnknownPath) {
    const QJsonObject params{{"name", "get_document"}, {"arguments", QJsonObject{{"path", "does/not/exist.html"}}}};
    const auto response = dispatcher->handle(makeRequest("tools/call", params));

    ASSERT_FALSE(response.isError()); // the JSON-RPC call itself succeeds...
    EXPECT_TRUE(response.result.toObject().contains("error")); // ...but the payload contains an error
}

} // namespace
