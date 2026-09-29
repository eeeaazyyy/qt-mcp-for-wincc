add_executable(winccmcp_tests
    main.cpp
    core/RagServiceTest.cpp
    infrastructure/JsonRpcDispatcherTest.cpp
    infrastructure/ParagraphChunkerTest.cpp
    infrastructure/SqliteVectorStoreTest.cpp
)

target_include_directories(winccmcp_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(winccmcp_tests PRIVATE
    winccmcp_core
    winccmcp_infrastructure
    GTest::gtest
    Qt6::Core
)

# SearchHistoryTableModel lives in winccmcp_app (the GUI target) - the test
# for it is only built together with the GUI.
if (WINCCMCP_BUILD_GUI)
    find_package(Qt6 REQUIRED COMPONENTS Test)
    target_sources(winccmcp_tests PRIVATE app/SearchHistoryTableModelTest.cpp)
    target_link_libraries(winccmcp_tests PRIVATE winccmcp_app Qt6::Test)
endif()

include(GoogleTest)
gtest_discover_tests(winccmcp_tests)

set_target_properties(winccmcp_tests PROPERTIES FOLDER "winccmcp")
