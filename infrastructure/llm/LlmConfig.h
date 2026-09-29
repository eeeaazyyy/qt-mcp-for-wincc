#ifndef WINCCMCP_INFRASTRUCTURE_LLM_LLMCONFIG_H__3E4F5A6B_7CCD_4CEE_B677_CDE1F2A3B4C5__INCLUDED_
#define WINCCMCP_INFRASTRUCTURE_LLM_LLMCONFIG_H__3E4F5A6B_7CCD_4CEE_B677_CDE1F2A3B4C5__INCLUDED_

#include <QString>

namespace winccmcp::infrastructure::llm {

// Connection settings for LM Studio (Local Server, OpenAI-compatible API).
// baseUrl is the origin only (scheme+host+port), no path: LmStudioClient
// appends "/v1/chat/completions" and "/v1/embeddings" itself.
struct LlmConfig {
    QString baseUrl = QStringLiteral("http://127.0.0.1:1234");
    QString chatModel = QStringLiteral("local-model");
    QString embedModel = QStringLiteral("text-embedding-nomic-embed-text-v1.5");
    QString apiKey = QStringLiteral("lm-studio");
    int timeoutSeconds = 120;

    // Reads LLM_BASE_URL / LLM_CHAT_MODEL / LLM_EMBED_MODEL / LLM_API_KEY from
    // environment variables (the same names as in the Python version's .env),
    // falling back to the defaults above when absent.
    static LlmConfig fromEnvironment();
};

} // namespace winccmcp::infrastructure::llm

#endif // WINCCMCP_INFRASTRUCTURE_LLM_LLMCONFIG_H__3E4F5A6B_7CCD_4CEE_B677_CDE1F2A3B4C5__INCLUDED_
