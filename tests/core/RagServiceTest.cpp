#include <gtest/gtest.h>

#include "core/services/RagService.h"
#include "tests/TestFakes.h"

using namespace winccmcp::core::services;
using namespace winccmcp::tests;

namespace {

TEST(RagServiceTest, RetrieveReturnsEmptyWhenNoVectorsStored) {
    FakeEmbeddingClient embedding;
    FakeChatClient chat;
    FakeVectorStore vectorStore;
    FakeDocumentRepository documentRepository;
    RagService service(embedding, chat, vectorStore, documentRepository);

    EXPECT_TRUE(service.retrieve("test query").isEmpty());
}

TEST(RagServiceTest, AnswerReturnsFallbackMessageWhenNothingFound) {
    FakeEmbeddingClient embedding;
    FakeChatClient chat;
    FakeVectorStore vectorStore;
    FakeDocumentRepository documentRepository;
    RagService service(embedding, chat, vectorStore, documentRepository);

    const auto answer = service.answer("what is dpGet?");

    EXPECT_TRUE(answer.sources.isEmpty());
    EXPECT_FALSE(answer.answer.isEmpty());
}

TEST(RagServiceTest, AnswerUsesLlmWithRetrievedContextAndReturnsSources) {
    FakeEmbeddingClient embedding;
    FakeChatClient chat;
    chat.cannedReply = "dpGet() reads a datapoint value [1]";
    FakeVectorStore vectorStore;
    FakeDocumentRepository documentRepository;

    winccmcp::core::models::RetrievedChunk chunk;
    chunk.documentPath = "ControlA_D/dpGet.html";
    chunk.documentTitle = "dpGet()";
    chunk.text = "Reads the value of a datapoint.";
    documentRepository.chunksById[42] = chunk;
    vectorStore.stored[42] = winccmcp::core::models::Embedding{1.0f, 0.0f, 0.0f, 0.0f};

    RagService service(embedding, chat, vectorStore, documentRepository);
    const auto answer = service.answer("How do I read a datapoint value?");

    EXPECT_EQ(answer.answer, chat.cannedReply);
    ASSERT_EQ(answer.sources.size(), 1);
    EXPECT_EQ(answer.sources.first().path, "ControlA_D/dpGet.html");

    ASSERT_EQ(chat.lastMessages.size(), 2);
    EXPECT_TRUE(chat.lastMessages.last().content.contains("Reads the value of a datapoint."));
}

} // namespace
