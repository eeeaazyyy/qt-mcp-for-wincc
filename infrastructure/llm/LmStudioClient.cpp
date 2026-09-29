#include "infrastructure/llm/LmStudioClient.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

#include <QJsonArray>
#include <QJsonDocument>

#include <httplib.h>

namespace winccmcp::infrastructure::llm {

using core::models::ChatRole;
using core::models::ChatTurns;
using core::models::Embedding;

namespace {

QString roleToString(ChatRole role) {
    switch (role) {
        case ChatRole::System: return QStringLiteral("system");
        case ChatRole::User: return QStringLiteral("user");
        case ChatRole::Assistant: return QStringLiteral("assistant");
    }
    return QStringLiteral("user");
}

} // namespace

LmStudioClient::LmStudioClient(LlmConfig config) : m_config(std::move(config)) {
}

QJsonObject LmStudioClient::postJson(const QString& path, const QJsonObject& body) const {
    httplib::Client client(m_config.baseUrl.toStdString());
    client.set_connection_timeout(m_config.timeoutSeconds, 0);
    client.set_read_timeout(m_config.timeoutSeconds, 0);
    client.set_write_timeout(m_config.timeoutSeconds, 0);

    const httplib::Headers headers = {
        {"Authorization", "Bearer " + m_config.apiKey.toStdString()},
    };

    const QByteArray payload = QJsonDocument(body).toJson(QJsonDocument::Compact);

    const auto response = client.Post(path.toStdString(), headers, payload.toStdString(), "application/json");
    if (!response) {
        throw std::runtime_error(
            "Could not connect to LM Studio (" + m_config.baseUrl.toStdString() + path.toStdString()
            + "): " + httplib::to_string(response.error())
            + ". Check that LM Studio is running and Local Server is enabled.");
    }
    if (response->status != 200) {
        throw std::runtime_error(
            "LM Studio returned HTTP " + std::to_string(response->status) + " on " + path.toStdString()
            + ": " + response->body);
    }

    const QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromStdString(response->body));
    if (!doc.isObject()) {
        throw std::runtime_error("LM Studio returned a non-JSON-object on " + path.toStdString());
    }
    return doc.object();
}

QString LmStudioClient::chat(const ChatTurns& messages) {
    QJsonArray messagesJson;
    for (const auto& turn : messages) {
        messagesJson.append(QJsonObject{{"role", roleToString(turn.role)}, {"content", turn.content}});
    }

    const QJsonObject body{
        {"model", m_config.chatModel},
        {"messages", messagesJson},
        {"temperature", 0.2},
        {"stream", false},
    };

    const QJsonObject response = postJson(QStringLiteral("/v1/chat/completions"), body);
    const QJsonArray choices = response.value("choices").toArray();
    if (choices.isEmpty()) {
        throw std::runtime_error("Response from /v1/chat/completions has no 'choices' field");
    }
    return choices.first().toObject().value("message").toObject().value("content").toString();
}

QVector<Embedding> LmStudioClient::embed(const QVector<QString>& texts) {
    if (texts.isEmpty()) {
        return {};
    }

    QJsonArray inputJson;
    for (const auto& text : texts) {
        inputJson.append(text);
    }

    const QJsonObject body{
        {"model", m_config.embedModel},
        {"input", inputJson},
    };

    const QJsonObject response = postJson(QStringLiteral("/v1/embeddings"), body);
    const QJsonArray data = response.value("data").toArray();
    if (data.isEmpty()) {
        throw std::runtime_error("Response from /v1/embeddings has no 'data' field");
    }

    // data may arrive out of order - sort by the index field just in case.
    QVector<QJsonObject> items;
    items.reserve(data.size());
    for (const auto& v : data) {
        items.push_back(v.toObject());
    }
    std::sort(items.begin(), items.end(), [](const QJsonObject& a, const QJsonObject& b) {
        return a.value("index").toInt() < b.value("index").toInt();
    });

    QVector<Embedding> embeddings;
    embeddings.reserve(items.size());
    for (const auto& item : items) {
        const QJsonArray vec = item.value("embedding").toArray();
        Embedding embedding;
        embedding.reserve(vec.size());
        for (const auto& component : vec) {
            embedding.push_back(static_cast<float>(component.toDouble()));
        }
        embeddings.push_back(embedding);
    }

    if (m_cachedEmbeddingDim < 0 && !embeddings.isEmpty()) {
        m_cachedEmbeddingDim = embeddings.first().size();
    }

    return embeddings;
}

int LmStudioClient::embeddingDimensions() {
    if (m_cachedEmbeddingDim < 0) {
        embed({QStringLiteral("winccoa mcp rag dimension probe")});
    }
    return m_cachedEmbeddingDim;
}

} // namespace winccmcp::infrastructure::llm
