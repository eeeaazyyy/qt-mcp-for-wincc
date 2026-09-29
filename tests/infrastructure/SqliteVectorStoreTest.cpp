#include <gtest/gtest.h>

#include <memory>

#include "infrastructure/persistence/SqliteConnection.h"
#include "infrastructure/persistence/SqliteEnvironment.h"
#include "infrastructure/persistence/SqliteSchema.h"
#include "infrastructure/persistence/SqliteVectorStore.h"

using namespace winccmcp::infrastructure::persistence;
using winccmcp::core::models::Embedding;

namespace {

class SqliteVectorStoreTest : public ::testing::Test {
protected:
    void SetUp() override {
        SqliteEnvironment::initializeOnce();
        connection = std::make_unique<SqliteConnection>(":memory:");
        SqliteSchema::ensureCreated(*connection, 4);
        store = std::make_unique<SqliteVectorStore>(*connection);
    }

    std::unique_ptr<SqliteConnection> connection;
    std::unique_ptr<SqliteVectorStore> store;
};

TEST_F(SqliteVectorStoreTest, SearchReturnsClosestVectorFirst) {
    store->insertChunkEmbedding(1, Embedding{1.0f, 0.0f, 0.0f, 0.0f});
    store->insertChunkEmbedding(2, Embedding{0.0f, 1.0f, 0.0f, 0.0f});
    store->insertChunkEmbedding(3, Embedding{0.9f, 0.1f, 0.0f, 0.0f});

    const auto matches = store->searchNearest(Embedding{1.0f, 0.0f, 0.0f, 0.0f}, 2);

    ASSERT_EQ(matches.size(), 2);
    EXPECT_EQ(matches.at(0).chunkId, 1);
    EXPECT_EQ(matches.at(1).chunkId, 3);
    EXPECT_LT(matches.at(0).distance, matches.at(1).distance);
}

TEST_F(SqliteVectorStoreTest, RemovedEmbeddingIsNotReturned) {
    store->insertChunkEmbedding(1, Embedding{1.0f, 0.0f, 0.0f, 0.0f});
    store->insertChunkEmbedding(2, Embedding{0.0f, 1.0f, 0.0f, 0.0f});

    store->removeChunkEmbedding(1);
    const auto matches = store->searchNearest(Embedding{1.0f, 0.0f, 0.0f, 0.0f}, 5);

    ASSERT_EQ(matches.size(), 1);
    EXPECT_EQ(matches.at(0).chunkId, 2);
}

} // namespace
