# sqlite-vec (https://github.com/asg017/sqlite-vec) is compiled directly into
# the application statically (not as a loadable .dll extension):
#   - SQLITE_CORE          -> sqlite-vec.c includes "sqlite3.h" directly and
#                              doesn't use the SQLITE_EXTENSION_INIT macros
#   - SQLITE_VEC_STATIC    -> SQLITE_VEC_API expands to nothing, no
#                              __declspec(dllexport)
# After that, registration happens once at process startup via
# sqlite3_auto_extension(sqlite3_vec_init) in SqliteEnvironment - every new
# sqlite3_open() connection automatically gets vec0 tables.
#
# sqlite-vec.h is generated from sqlite-vec.h.tmpl (a template with ${VERSION}
# etc. - upstream does this with `envsubst` from the Makefile; here CMake
# itself does it via configure_file, since it also substitutes ${VAR}).

set(SQLITE_VEC_DIR ${SQLITE_VEC_SOURCE_DIR})

set(VERSION "0.1.9")
set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "9")
set(DATE "${CMAKE_CURRENT_LIST_FILE}")   # doesn't affect functionality, just diagnostics in SQLITE_VEC_DATE
set(SOURCE "vendored-via-fetchcontent")  # likewise, just diagnostics in SQLITE_VEC_SOURCE

# No @ONLY: the template uses ${VAR} syntax (like envsubst upstream), not
# @VAR@, so a regular (non-@ONLY) configure_file substitution is needed.
configure_file(
    ${SQLITE_VEC_DIR}/sqlite-vec.h.tmpl
    ${CMAKE_CURRENT_BINARY_DIR}/generated/sqlite-vec.h
)

add_library(sqlite_vec STATIC
    ${SQLITE_VEC_DIR}/sqlite-vec.c
)

target_include_directories(sqlite_vec PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/generated
)

# PUBLIC, not PRIVATE: sqlite-vec.h itself checks SQLITE_CORE to decide
# whether to include "sqlite3ext.h" (the loadable-extension branch, with
# macros like sqlite3_auto_extension -> sqlite3_api->auto_extension) or plain
# "sqlite3.h". Anyone who just #include "sqlite-vec.h" (e.g.
# SqliteEnvironment.cpp, outside this library) must see the same definition -
# otherwise the build fails on an undeclared sqlite3_api.
target_compile_definitions(sqlite_vec PUBLIC
    SQLITE_CORE
    SQLITE_VEC_STATIC
)

target_link_libraries(sqlite_vec PUBLIC sqlite3::sqlite3)

if (MSVC)
    target_compile_options(sqlite_vec PRIVATE /w)
else()
    target_compile_options(sqlite_vec PRIVATE -w)
endif()

add_library(sqlite_vec::sqlite_vec ALIAS sqlite_vec)
