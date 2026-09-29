add_executable(qt-mcp-for-wincc main.cpp)

target_link_libraries(qt-mcp-for-wincc PRIVATE
    winccmcp_app
    winccmcp_infrastructure
    winccmcp_core
    Qt6::Widgets
)

target_compile_definitions(qt-mcp-for-wincc PRIVATE
    WINCCMCP_DEFAULT_DOCS_DIR="${CMAKE_SOURCE_DIR}/en_US"
    WINCCMCP_DEFAULT_DB_PATH="${CMAKE_SOURCE_DIR}/data/winccoa_docs.sqlite3"
)

set_target_properties(qt-mcp-for-wincc PROPERTIES FOLDER "winccmcp")

# Translations: i18n/*.ts (except English - it's the source language of the
# code) are compiled into .qm and copied next to the exe, from where
# TranslationManager reads them.
qt6_add_translation(WINCCMCP_QM_FILES ${CMAKE_SOURCE_DIR}/i18n/winccmcp_ru.ts)

add_custom_target(winccmcp_translations DEPENDS ${WINCCMCP_QM_FILES})
add_dependencies(qt-mcp-for-wincc winccmcp_translations)

foreach(qm_file IN LISTS WINCCMCP_QM_FILES)
    add_custom_command(TARGET qt-mcp-for-wincc POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:qt-mcp-for-wincc>/i18n"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${qm_file}" "$<TARGET_FILE_DIR:qt-mcp-for-wincc>/i18n/"
    )
endforeach()
