add_library(winccmcp_core STATIC
    # models/interfaces - header-only, listed explicitly for IDE convenience
    interfaces/IChatHistoryRepository.h
    interfaces/IDocumentRepository.h
    interfaces/IEmbeddingClient.h
    interfaces/IHtmlDocumentParser.h
    interfaces/IJsonRpcHandler.h
    interfaces/IJsonRpcTransport.h
    interfaces/ILlmChatClient.h
    interfaces/ITextChunker.h
    interfaces/IVectorStore.h

    models/ChatHistoryRecord.h
    models/ChatTurn.h
    models/Chunk.h
    models/Document.h
    models/JsonRpc.h
    models/RagAnswer.h
    models/RetrievedChunk.h
    models/Source.h
    models/VectorMatch.h

    services/IngestionService.h
    services/IngestionService.cpp
    services/McpToolRegistry.h
    services/McpToolRegistry.cpp
    services/RagService.h
    services/RagService.cpp
)

target_include_directories(winccmcp_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..   # so #include "core/..." works
)

target_link_libraries(winccmcp_core PUBLIC Qt6::Core)

set_target_properties(winccmcp_core PROPERTIES FOLDER "winccmcp")
