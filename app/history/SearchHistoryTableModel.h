#ifndef WINCCMCP_APP_HISTORY_SEARCHHISTORYTABLEMODEL_H__B0C1D2E3_F445_4566_3DEF_E8F9A0B1C2D3__INCLUDED_
#define WINCCMCP_APP_HISTORY_SEARCHHISTORYTABLEMODEL_H__B0C1D2E3_F445_4566_3DEF_E8F9A0B1C2D3__INCLUDED_

#include <QAbstractTableModel>

#include "core/models/ChatHistoryRecord.h"

namespace winccmcp::app::history {

class SearchHistoryTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    enum Column { TimeColumn = 0, QueryColumn, DurationColumn, ColumnCount };

    explicit SearchHistoryTableModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void setRecords(const core::models::ChatHistoryRecords& records);
    void addRecord(const core::models::ChatHistoryRecord& record);

    const core::models::ChatHistoryRecord& recordAt(int row) const;

private:
    core::models::ChatHistoryRecords m_records;
};

} // namespace winccmcp::app::history

#endif // WINCCMCP_APP_HISTORY_SEARCHHISTORYTABLEMODEL_H__B0C1D2E3_F445_4566_3DEF_E8F9A0B1C2D3__INCLUDED_
