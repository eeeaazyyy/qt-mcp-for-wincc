#include "app/history/HistoryController.h"

#include "app/history/ChatHistoryDialog.h"
#include "app/history/HistoryView.h"
#include "app/history/SearchHistoryTableModel.h"

namespace winccmcp::app::history {

HistoryController::HistoryController(HistoryView& view, SearchHistoryTableModel& model, QObject* parent)
    : QObject(parent)
    , m_view(view)
    , m_model(model) {
    connect(&m_view, &HistoryView::rowActivated, this, &HistoryController::onRowActivated);
}

void HistoryController::onRowActivated(int row) {
    if (row < 0 || row >= m_model.rowCount()) {
        return;
    }
    ChatHistoryDialog dialog(m_model.recordAt(row), &m_view);
    dialog.exec();
}

} // namespace winccmcp::app::history
