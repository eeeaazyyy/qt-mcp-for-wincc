#include "app/mainwindow/MainWindow.h"

#include <exception>
#include <utility>

#include <QActionGroup>
#include <QEvent>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMetaObject>
#include <QProgressDialog>
#include <QTabWidget>
#include <QtConcurrentRun>

#include "app/chat/ChatController.h"
#include "app/chat/ChatView.h"
#include "app/common/MessagedException.h"
#include "app/common/TranslationManager.h"
#include "app/history/HistoryController.h"
#include "app/history/HistoryView.h"
#include "app/history/SearchHistoryTableModel.h"

namespace winccmcp::app::mainwindow {

using core::services::IngestionService;

MainWindow::MainWindow(core::services::RagService& ragService,
                        core::services::IngestionService& ingestionService,
                        core::interfaces::IChatHistoryRepository& historyRepository,
                        QString docsRootDir,
                        common::TranslationManager& translationManager,
                        QWidget* parent)
    : QMainWindow(parent)
    , m_ingestionService(ingestionService)
    , m_docsRootDir(std::move(docsRootDir))
    , m_translationManager(translationManager) {
    m_historyModel = new history::SearchHistoryTableModel(this);
    m_historyModel->setRecords(historyRepository.loadAll());

    m_chatView = new chat::ChatView(this);
    m_historyView = new history::HistoryView(*m_historyModel, this);

    m_chatController = new chat::ChatController(*m_chatView, ragService, historyRepository, *m_historyModel, this);
    m_historyController = new history::HistoryController(*m_historyView, *m_historyModel, this);

    m_tabs = new QTabWidget(this);
    m_tabs->addTab(m_chatView, QString());
    m_tabs->addTab(m_historyView, QString());
    setCentralWidget(m_tabs);

    m_indexingMenu = menuBar()->addMenu(QString());
    m_reindexAction = m_indexingMenu->addAction(QString());
    connect(m_reindexAction, &QAction::triggered, this, &MainWindow::runIngestion);

    m_languageMenu = menuBar()->addMenu(QString());
    auto* languageGroup = new QActionGroup(this);
    languageGroup->setExclusive(true);
    for (const QString& code : m_translationManager.availableLanguages()) {
        QAction* action = m_languageMenu->addAction(QString());
        action->setCheckable(true);
        action->setChecked(code == m_translationManager.currentLanguage());
        action->setData(code);
        languageGroup->addAction(action);
        connect(action, &QAction::triggered, this, [this, code] { m_translationManager.switchLanguage(code); });
    }

    connect(&m_ingestWatcher, &QFutureWatcher<IngestionService::Result>::finished,
                                                this, &MainWindow::onIngestionFinished);

    resize(1000, 700);
    retranslateUi();
}

void MainWindow::runIngestion() {
    m_reindexAction->setEnabled(false);

    m_progressDialog = new QProgressDialog(tr("Indexing documentation"), tr("Cancel"), 0, 0, this);
    m_progressDialog->setWindowModality(Qt::WindowModal);
    m_progressDialog->setMinimumDuration(0);
    m_progressDialog->show();

    const IngestionService::ProgressCallback progressCallback = [this](int current, int total, const QString& path) {
        QMetaObject::invokeMethod(
            this, [this, current, total, path] { onIngestionProgress(current, total, path); }, Qt::QueuedConnection);
    };

    m_ingestWatcher.setFuture(QtConcurrent::run([this, progressCallback]() -> IngestionService::Result {
        try {
            return m_ingestionService.run(m_docsRootDir, progressCallback);
        } catch (const std::exception& e) {
            throw common::MessagedException(QString::fromUtf8(e.what()));
        }
    }));
}

void MainWindow::onIngestionProgress(int current, int total, const QString& relativePath) {
    if (!m_progressDialog) {
        return;
    }
    if (m_progressDialog->maximum() != total) {
        m_progressDialog->setMaximum(total);
    }
    m_progressDialog->setValue(current);
    m_progressDialog->setLabelText(tr("Processing: %1 (%2 of %3)").arg(relativePath).arg(current).arg(total));
}

void MainWindow::onIngestionFinished() {
    IngestionService::Result result;
    QString errorMessage;
    try {
        result = m_ingestWatcher.result();
    } catch (const std::exception& e) {
        errorMessage = QString::fromUtf8(e.what());
    }

    if (m_progressDialog) {
        m_progressDialog->close();
        m_progressDialog->deleteLater();
        m_progressDialog = nullptr;
    }
    m_reindexAction->setEnabled(true);

    if (!errorMessage.isEmpty()) {
        QMessageBox::critical(this, tr("Indexing documentation"),
                               tr("Indexing was interrupted by an error:\n%1").arg(errorMessage));
        return;
    }

    QMessageBox::information(this, tr("Indexing documentation"),
                        tr("Indexing complete: %1 documents, %2 reindexed, %3 unchanged, %4 chunks.")
                                  .arg(result.parsedDocuments)
                                  .arg(result.reindexedDocuments)
                                  .arg(result.skippedUnchanged)
                                  .arg(result.totalChunks));
}

void MainWindow::changeEvent(QEvent* event) {
    QMainWindow::changeEvent(event);
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();
    }
}

void MainWindow::retranslateUi() {
    setWindowTitle(tr("Qt MCP for WinCC OA"));
    m_tabs->setTabText(0, tr("Chat"));
    m_tabs->setTabText(1, tr("Search history"));
    m_indexingMenu->setTitle(tr("Indexing"));
    m_reindexAction->setText(tr("Build index..."));
    m_languageMenu->setTitle(tr("Language"));

    const auto actions = m_languageMenu->actions();
    for (QAction* action : actions) {
        const QString code = action->data().toString();
        action->setText(code == QStringLiteral("ru") ? tr("Russian") : tr("English"));
    }
}

} // namespace winccmcp::app::mainwindow
