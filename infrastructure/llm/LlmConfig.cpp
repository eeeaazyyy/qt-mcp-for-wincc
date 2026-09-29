#include "infrastructure/llm/LlmConfig.h"

#include <QProcessEnvironment>

namespace winccmcp::infrastructure::llm {

LlmConfig LlmConfig::fromEnvironment() {
    const auto env = QProcessEnvironment::systemEnvironment();
    LlmConfig config;

    config.baseUrl = env.value(QStringLiteral("LLM_BASE_URL"), config.baseUrl);
    config.chatModel = env.value(QStringLiteral("LLM_CHAT_MODEL"), config.chatModel);
    config.embedModel = env.value(QStringLiteral("LLM_EMBED_MODEL"), config.embedModel);
    config.apiKey = env.value(QStringLiteral("LLM_API_KEY"), config.apiKey);

    return config;
}

} // namespace winccmcp::infrastructure::llm
