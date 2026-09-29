add_library(winccmcp_infrastructure STATIC
    llm/LlmConfig.h
    llm/LlmConfig.cpp
    llm/LmStudioClient.h
    llm/LmStudioClient.cpp

    parsing/LexborHtmlParser.h
    parsing/LexborHtmlParser.cpp
    parsing/ParagraphChunker.h
    parsing/ParagraphChunker.cpp

    persistence/SqliteChatHistoryRepository.h
    persistence/SqliteChatHistoryRepository.cpp
    persistence/SqliteConnection.h
    persistence/SqliteConnection.cpp
    persistence/SqliteDocumentRepository.h
    persistence/SqliteDocumentRepository.cpp
    persistence/SqliteEnvironment.h
    persistence/SqliteEnvironment.cpp
    persistence/SqliteSchema.h
    persistence/SqliteSchema.cpp
    persistence/SqliteStatement.h
    persistence/SqliteStatement.cpp
    persistence/SqliteVectorStore.h
    persistence/SqliteVectorStore.cpp

    mcp/HttpJsonRpcTransport.h
    mcp/HttpJsonRpcTransport.cpp
    mcp/JsonRpcDispatcher.h
    mcp/JsonRpcDispatcher.cpp
)

target_include_directories(winccmcp_infrastructure PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(winccmcp_infrastructure PUBLIC
    winccmcp_core
    Qt6::Core
    sqlite3::sqlite3
    sqlite_vec::sqlite_vec
    lexbor::lexbor
    httplib::httplib
)

# std::jthread (HttpJsonRpcTransport) needs runtime thread support.
find_package(Threads REQUIRED)
target_link_libraries(winccmcp_infrastructure PUBLIC Threads::Threads)

set_target_properties(winccmcp_infrastructure PROPERTIES FOLDER "winccmcp")
