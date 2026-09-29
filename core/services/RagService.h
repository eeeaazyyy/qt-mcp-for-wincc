#ifndef WINCCMCP_CORE_SERVICES_RAGSERVICE_H__D2E3F4A5_B612_4223_FAB5_0C1D2E3F4A51__INCLUDED_
#define WINCCMCP_CORE_SERVICES_RAGSERVICE_H__D2E3F4A5_B612_4223_FAB5_0C1D2E3F4A51__INCLUDED_

#include <QString>

#include "core/interfaces/IDocumentRepository.h"
#include "core/interfaces/IEmbeddingClient.h"
#include "core/interfaces/ILlmChatClient.h"
#include "core/interfaces/IVectorStore.h"
#include "core/models/RagAnswer.h"
#include "core/models/RetrievedChunk.h"

namespace winccmcp::core::services {

// Facade: the single entry point for the GUI and for the search_docs/ask_docs
// JSON-RPC tools. Depends only on interfaces (DIP) - which LLM/vector store
// is actually used doesn't matter to RagService.
class RagService {
public:
    RagService(interfaces::IEmbeddingClient& embeddingClient,
               interfaces::ILlmChatClient& chatClient,
               interfaces::IVectorStore& vectorStore,
               interfaces::IDocumentRepository& documentRepository,
               int defaultTopK = 5);

    // Plain semantic search, without calling the LLM (search_docs).
    models::RetrievedChunks retrieve(const QString& query, int topK = -1);

    // Search + generation of a coherent answer with source citations (ask_docs).
    models::RagAnswer answer(const QString& question, int topK = -1);

private:
    interfaces::IEmbeddingClient& m_embeddingClient;
    interfaces::ILlmChatClient& m_chatClient;
    interfaces::IVectorStore& m_vectorStore;
    interfaces::IDocumentRepository& m_documentRepository;
    int m_defaultTopK;
};

} // namespace winccmcp::core::services

#endif // WINCCMCP_CORE_SERVICES_RAGSERVICE_H__D2E3F4A5_B612_4223_FAB5_0C1D2E3F4A51__INCLUDED_
