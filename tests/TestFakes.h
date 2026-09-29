#ifndef WINCCMCP_TESTS_TESTFAKES_H__D8E9FA0B_1CCD_4CDE_B567_A6B7C8D9EAFB__INCLUDED_
#define WINCCMCP_TESTS_TESTFAKES_H__D8E9FA0B_1CCD_4CDE_B567_A6B7C8D9EAFB__INCLUDED_

#include <QMap>

#include "core/interfaces/IDocumentRepository.h"
#include "core/interfaces/IEmbeddingClient.h"
#include "core/interfaces/ILlmChatClient.h"
#include "core/interfaces/IVectorStore.h"

// Lightweight fakes of the core:: interfaces for unit tests of core::services
// and infrastructure::mcp - no gmock (per the decision in Dependencies.cmake:
// BUILD_GMOCK OFF), just minimal implementations with predictable behavior.
namespace winccmcp::tests {

class FakeEmbeddingClient : public core::interfaces::IEmbeddingClient {
public:
    int dimensions = 4;

    QVector<core::models::Embedding> embed(const QVector<QString>& texts) override {
        QVector<core::models::Embedding> result;
        for (const auto& text : texts) {
            core::models::Embedding e(dimensions, 0.0f);
            e[0] = static_cast<float>(text.size());
            result.push_back(e);
        }
        return result;
    }

    int embeddingDimensions() override { return dimensions; }
};

class FakeChatClient : public core::interfaces::ILlmChatClient {
public:
    QString cannedReply = QStringLiteral("fake answer");
    core::models::ChatTurns lastMessages;

    QString chat(const core::models::ChatTurns& messages) override {
        lastMessages = messages;
        return cannedReply;
    }
};

class FakeVectorStore : public core::interfaces::IVectorStore {
public:
    QMap<qint64, core::models::Embedding> stored;

    void insertChunkEmbedding(qint64 chunkId, const core::models::Embedding& embedding) override {
        stored[chunkId] = embedding;
    }

    void removeChunkEmbedding(qint64 chunkId) override { stored.remove(chunkId); }

    core::models::VectorMatches searchNearest(const core::models::Embedding& /*queryEmbedding*/, int topK) override {
        core::models::VectorMatches matches;
        int i = 0;
        for (auto it = stored.begin(); it != stored.end() && i < topK; ++it, ++i) {
            matches.push_back(core::models::VectorMatch{it.key(), static_cast<double>(i)});
        }
        return matches;
    }
};

class FakeDocumentRepository : public core::interfaces::IDocumentRepository {
public:
    QMap<QString, core::models::Document> documentsByPath;
    QMap<qint64, core::models::RetrievedChunk> chunksById;

    std::optional<core::models::Document> findByPath(const QString& path) override {
        const auto it = documentsByPath.find(path);
        return it == documentsByPath.end() ? std::nullopt : std::optional(it.value());
    }

    QVector<core::models::Document> search(const QString& substring, int limit) override {
        QVector<core::models::Document> result;
        for (const auto& doc : documentsByPath) {
            if (doc.title.contains(substring) || doc.path.contains(substring)) {
                result.push_back(doc);
            }
            if (result.size() >= limit) break;
        }
        return result;
    }

    std::pair<qint64, bool> upsertDocument(const core::models::Document& doc) override {
        const bool changed = !documentsByPath.contains(doc.path)
                              || documentsByPath[doc.path].contentHash != doc.contentHash;
        documentsByPath[doc.path] = doc;
        return {documentsByPath.size(), changed};
    }

    QVector<qint64> chunkIdsForDocument(qint64 /*documentId*/) override { return {}; }

    QVector<core::models::Chunk> replaceChunks(qint64 documentId, const QVector<QString>& chunkTexts) override {
        QVector<core::models::Chunk> chunks;
        for (int i = 0; i < chunkTexts.size(); ++i) {
            core::models::Chunk c;
            c.id = documentId * 1000 + i;
            c.documentId = documentId;
            c.chunkIndex = i;
            c.text = chunkTexts.at(i);
            chunks.push_back(c);
        }
        return chunks;
    }

    core::models::RetrievedChunks resolveChunks(const core::models::VectorMatches& matches) override {
        core::models::RetrievedChunks result;
        for (const auto& m : matches) {
            if (chunksById.contains(m.chunkId)) {
                auto chunk = chunksById.value(m.chunkId);
                chunk.distance = m.distance;
                result.push_back(chunk);
            }
        }
        return result;
    }
};

} // namespace winccmcp::tests

#endif // WINCCMCP_TESTS_TESTFAKES_H__D8E9FA0B_1CCD_4CDE_B567_A6B7C8D9EAFB__INCLUDED_
