# app/ depends only on core (interfaces/services), not on infrastructure -
# the concrete adapters (LM Studio, SQLite, HTTP) are only visible to
# app_main (the composition root). This way the GUI could be tested against
# fake implementations of the interfaces, without touching Qt networking or SQLite.
add_library(winccmcp_app STATIC
    common/MessagedException.h
    common/TranslationManager.h
    common/TranslationManager.cpp

    mainwindow/MainWindow.h
    mainwindow/MainWindow.cpp

    chat/ChatController.h
    chat/ChatController.cpp
    chat/ChatMessage.h
    chat/ChatMessageDelegate.h
    chat/ChatMessageDelegate.cpp
    chat/ChatMessageListModel.h
    chat/ChatMessageListModel.cpp
    chat/ChatView.h
    chat/ChatView.cpp

    history/ChatHistoryDialog.h
    history/ChatHistoryDialog.cpp
    history/HistoryController.h
    history/HistoryController.cpp
    history/HistoryTableItemDelegate.h
    history/HistoryTableItemDelegate.cpp
    history/HistoryView.h
    history/HistoryView.cpp
    history/SearchHistoryTableModel.h
    history/SearchHistoryTableModel.cpp
)

target_include_directories(winccmcp_app PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(winccmcp_app PUBLIC
    winccmcp_core
    Qt6::Widgets
    Qt6::Concurrent
)

set_target_properties(winccmcp_app PROPERTIES FOLDER "winccmcp")
