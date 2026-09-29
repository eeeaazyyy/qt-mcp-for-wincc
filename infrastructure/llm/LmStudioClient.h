#ifndef WINCCMCP_INFRASTRUCTURE_LLM_LMSTUDIOCLIENT_H__4F5A6B7C_8DDE_4DFF_C788_DEF2A3B4C5D6__INCLUDED_
#define WINCCMCP_INFRASTRUCTURE_LLM_LMSTUDIOCLIENT_H__4F5A6B7C_8DDE_4DFF_C788_DEF2A3B4C5D6__INCLUDED_

#include <QJsonObject>

#include "core/interfaces/IEmbeddingClient.h"
#include "core/interfaces/ILlmChatClient.h"
#include "infrastructure/llm/LlmConfig.h"

namespace winccmcp::infrastructure::llm {

// A single implementation of both interfaces (Adapter): the same local
// LM Studio server serves both chat and embeddings over an OpenAI-compatible
// protocol. Built on httplib::Client (blocking calls) - the same library
// used for the HTTP server in infrastructure::mcp, so the project doesn't
// need a second HTTP library.
//
// A blocking call is safe on HttpJsonRpcTransport's worker threads; from the
// GUI thread these methods are only called via QtConcurrent::run() -
// see app::chat::ChatController.
class LmStudioClient : public core::interfaces::ILlmChatClient, public core::interfaces::IEmbeddingClient {
public:
    explicit LmStudioClient(LlmConfig config);

    QString chat(const core::models::ChatTurns& messages) override;

    QVector<core::models::Embedding> embed(const QVector<QString>& texts) override;
    int embeddingDimensions() override;

private:
    LlmConfig m_config;
    int m_cachedEmbeddingDim = -1;

    QJsonObject postJson(const QString& path, const QJsonObject& body) const;
};

} // namespace winccmcp::infrastructure::llm

#endif // WINCCMCP_INFRASTRUCTURE_LLM_LMSTUDIOCLIENT_H__4F5A6B7C_8DDE_4DFF_C788_DEF2A3B4C5D6__INCLUDED_
