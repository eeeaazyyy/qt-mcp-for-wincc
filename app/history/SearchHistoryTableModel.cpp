#include "app/history/SearchHistoryTableModel.h"

namespace winccmcp::app::history {

using core::models::ChatHistoryRecord;
using core::models::ChatHistoryRecords;

SearchHistoryTableModel::SearchHistoryTableModel(QObject* parent) : QAbstractTableModel(parent) {
}

int SearchHistoryTableModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : m_records.size();
}

int SearchHistoryTableModel::columnCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : ColumnCount;
}

QVariant SearchHistoryTableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_records.size()) {
        return {};
    }
    const ChatHistoryRecord& record = m_records.at(index.row());

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case TimeColumn:
                return record.timestamp.toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"));
            case QueryColumn:
                return record.query;
            case DurationColumn:
                return record.durationMs;
            default:
                return {};
        }
    }

    if (role == Qt::ToolTipRole && index.column() == QueryColumn) {
        return record.query;
    }

    if (role == Qt::TextAlignmentRole && index.column() == DurationColumn) {
        return QVariant::fromValue(static_cast<int>(Qt::AlignRight | Qt::AlignVCenter));
    }

    return {};
}

QVariant SearchHistoryTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
        return QAbstractTableModel::headerData(section, orientation, role);
    }
    switch (section) {
        case TimeColumn:
            return tr("Time");
        case QueryColumn:
            return tr("Query");
        case DurationColumn:
            return tr("Response time, ms");
        default:
            return {};
    }
}

void SearchHistoryTableModel::setRecords(const ChatHistoryRecords& records) {
    beginResetModel();
    m_records = records;
    endResetModel();
}

void SearchHistoryTableModel::addRecord(const ChatHistoryRecord& record) {
    const int row = m_records.size();
    beginInsertRows(QModelIndex(), row, row);
    m_records.push_back(record);
    endInsertRows();
}

const ChatHistoryRecord& SearchHistoryTableModel::recordAt(int row) const {
    return m_records.at(row);
}

} // namespace winccmcp::app::history
