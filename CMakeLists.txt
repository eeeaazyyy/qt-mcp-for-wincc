cmake_minimum_required(VERSION 3.24)

project(qt_mcp_for_wincc
    VERSION 0.1.0
    DESCRIPTION "Qt MCP server with RAG over the WinCC OA documentation"
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)  # UI is written by hand (widgets in code), no .ui files

# Qt6 is installed separately (Qt Online Installer / aqtinstall) - see README.md.
# If find_package below doesn't find Qt, pass the path explicitly:
#   cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.7.2/msvc2019_64"
option(WINCCMCP_BUILD_TESTS "Build unit tests (GoogleTest)" ON)
option(WINCCMCP_BUILD_GUI "Build the Qt GUI (winccmcp_app, app_main)" ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Qt6::Core is used even in core/ (QJsonValue as a plain JSON value-type, no
# GUI, no event loop) - so it's looked up unconditionally, not just for the GUI.
find_package(Qt6 REQUIRED COMPONENTS Core)

include(cmake/Dependencies.cmake)

add_subdirectory(third_party/sqlite-vec)
add_subdirectory(core)
add_subdirectory(infrastructure)

if (WINCCMCP_BUILD_GUI)
    find_package(Qt6 REQUIRED COMPONENTS Widgets Concurrent LinguistTools)
    add_subdirectory(app)
    add_subdirectory(app_main)
endif()

if (WINCCMCP_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
