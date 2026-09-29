# External dependencies via CMake FetchContent (no Conan/vcpkg).
#
# Qt6 is deliberately not included here - building Qt from source for hours
# isn't practical for a learning project. Qt6 is installed by the user
# separately (Qt Online Installer / aqtinstall) and found via the usual
# find_package(Qt6) in the root CMakeLists.txt through CMAKE_PREFIX_PATH.
#
# The first configuration of the project downloads the sources (needs
# internet); subsequent ones use the cache in build/_deps.

include(FetchContent)

set(FETCHCONTENT_QUIET OFF)

# --- sqlite3 (amalgamation: sqlite3.c + sqlite3.h in a single file) -----
FetchContent_Declare(
    sqlite3
    URL https://www.sqlite.org/2026/sqlite-amalgamation-3530400.zip
    URL_HASH SHA3_256=628a44cfe82c66aed1ccbbe85a562d2e33ebe64b3288981ed76285612227934e
)
FetchContent_MakeAvailable(sqlite3)

add_library(sqlite3 STATIC ${sqlite3_SOURCE_DIR}/sqlite3.c)
target_include_directories(sqlite3 PUBLIC ${sqlite3_SOURCE_DIR})
target_compile_definitions(sqlite3 PUBLIC
    SQLITE_ENABLE_FTS5
    SQLITE_THREADSAFE=1
)
if (CMAKE_C_COMPILER_ID STREQUAL "MSVC" OR CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(sqlite3 PRIVATE /w)
else()
    target_compile_options(sqlite3 PRIVATE -w)
endif()
add_library(sqlite3::sqlite3 ALIAS sqlite3)

# --- sqlite-vec (sqlite-vec.c/.h amalgamation, compiled in statically) --
FetchContent_Declare(
    sqlite_vec
    GIT_REPOSITORY https://github.com/asg017/sqlite-vec.git
    GIT_TAG v0.1.9
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(sqlite_vec)
# The build is wrapped in third_party/sqlite-vec/CMakeLists.txt (generating
# .h from .tmpl + an object library) - included right here since the
# directory is already known.
set(SQLITE_VEC_SOURCE_DIR ${sqlite_vec_SOURCE_DIR} CACHE INTERNAL "")

# --- lexbor (HTML5 parser with CSS selectors) ---------------------------
set(LEXBOR_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(LEXBOR_BUILD_STATIC ON CACHE BOOL "" FORCE)
set(LEXBOR_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(LEXBOR_BUILD_TESTS_CPP OFF CACHE BOOL "" FORCE)
set(LEXBOR_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(LEXBOR_BUILD_UTILS OFF CACHE BOOL "" FORCE)
set(LEXBOR_BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    lexbor
    GIT_REPOSITORY https://github.com/lexbor/lexbor.git
    GIT_TAG v3.0.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(lexbor)
if (NOT TARGET lexbor::lexbor)
    add_library(lexbor::lexbor ALIAS lexbor_static)
endif()

# --- cpp-httplib (HTTP client + server, header-only) ---------------------
set(HTTPLIB_USE_OPENSSL_IF_AVAILABLE OFF CACHE BOOL "" FORCE)
set(HTTPLIB_USE_ZLIB_IF_AVAILABLE OFF CACHE BOOL "" FORCE)
set(HTTPLIB_USE_BROTLI_IF_AVAILABLE OFF CACHE BOOL "" FORCE)
set(HTTPLIB_USE_ZSTD_IF_AVAILABLE OFF CACHE BOOL "" FORCE)
set(HTTPLIB_COMPILE OFF CACHE BOOL "" FORCE)
set(HTTPLIB_TEST OFF CACHE BOOL "" FORCE)
set(HTTPLIB_INSTALL OFF CACHE BOOL "" FORCE)
# MinGW-w64 doesn't declare GetAddrInfoExCancel in its Windows SDK headers,
# which cpp-httplib's non-blocking getaddrinfo relies on - disable it, it's
# not critical for us (localhost, one-off requests to LM Studio).
set(HTTPLIB_USE_NON_BLOCKING_GETADDRINFO OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    cpp_httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.56.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(cpp_httplib)

# --- GoogleTest (unit tests) ---------------------------------------------
if (WINCCMCP_BUILD_TESTS)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.18.0
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(googletest)
endif()
