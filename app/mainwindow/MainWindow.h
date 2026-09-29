#ifndef WINCCMCP_APP_MAINWINDOW_MAINWINDOW_H__C7D8E9FA_0BBC_4BCD_A456_F5A6B7C8D9EA__INCLUDED_
#define WINCCMCP_APP_MAINWINDOW_MAINWINDOW_H__C7D8E9FA_0BBC_4BCD_A456_F5A6B7C8D9EA__INCLUDED_

#include <QFutureWatcher>
#include <QMainWindow>

#include "core/interfaces/IChatHistoryRepository.h"
#include "core/services/IngestionService.h"
#include "core/services/RagService.h"

class QAction;
class QMenu;
class QProgressDialog;
class QTabWidget;

namespace winccmcp::app::common {
class TranslationManager;
} // namespace winccmcp::app::common

namespace winccmcp::app::chat {
class ChatView;
class ChatController;
} // namespace winccmcp::app::chat

namespace winccmcp::app::history {
class HistoryView;
class HistoryController;
class SearchHistoryTableModel;
} // namespace winccmcp::app::history

namespace winccmcp::app::mainwindow {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(core::services::RagService& ragService,
               core::services::IngestionService& ingestionService,
               core::interfaces::IChatHistoryRepository& historyRepository,
               QString docsRootDir,
               common::TranslationManager& translationManager,
               QWidget* parent = nullptr);

protected:
    void changeEvent(QEvent* event) override;

private:
    void retranslateUi();
    void runIngestion();
    void onIngestionFinished();
    void onIngestionProgress(int current, int total, const QString& relativePath);

    core::services::IngestionService& m_ingestionService;
    QString m_docsRootDir;
    common::TranslationManager& m_translationManager;

    QTabWidget* m_tabs;
    chat::ChatView* m_chatView;
    history::HistoryView* m_historyView;
    history::SearchHistoryTableModel* m_historyModel;
    chat::ChatController* m_chatController;
    history::HistoryController* m_historyController;

    QMenu* m_indexingMenu;
    QAction* m_reindexAction;
    QMenu* m_languageMenu;

    QFutureWatcher<core::services::IngestionService::Result> m_ingestWatcher;
    QProgressDialog* m_progressDialog = nullptr;
};

} // namespace winccmcp::app::mainwindow

#endif // WINCCMCP_APP_MAINWINDOW_MAINWINDOW_H__C7D8E9FA_0BBC_4BCD_A456_F5A6B7C8D9EA__INCLUDED_
