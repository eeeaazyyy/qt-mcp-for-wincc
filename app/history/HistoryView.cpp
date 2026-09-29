#include "app/history/HistoryView.h"

#include <QHeaderView>
#include <QTableView>
#include <QVBoxLayout>

#include "app/history/HistoryTableItemDelegate.h"
#include "app/history/SearchHistoryTableModel.h"

namespace winccmcp::app::history {

HistoryView::HistoryView(SearchHistoryTableModel& model, QWidget* parent) : QWidget(parent) {
    m_tableView = new QTableView(this);
    m_tableView->setModel(&model);
    m_tableView->setItemDelegate(new HistoryTableItemDelegate(m_tableView));
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    m_tableView->horizontalHeader()->setStretchLastSection(false);
    m_tableView->horizontalHeader()->
                setSectionResizeMode(SearchHistoryTableModel::QueryColumn, QHeaderView::Stretch);
    m_tableView->horizontalHeader()->
                setSectionResizeMode(SearchHistoryTableModel::TimeColumn, QHeaderView::ResizeToContents);
    m_tableView->horizontalHeader()->
                setSectionResizeMode(SearchHistoryTableModel::DurationColumn, QHeaderView::ResizeToContents);
    m_tableView->verticalHeader()->setVisible(false);

    connect(m_tableView, &QTableView::doubleClicked, this,
            [this](const QModelIndex& index) { emit rowActivated(index.row()); });

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(m_tableView);
}

} // namespace winccmcp::app::history
