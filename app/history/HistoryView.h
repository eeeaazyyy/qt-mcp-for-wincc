#ifndef WINCCMCP_APP_HISTORY_HISTORYVIEW_H__D2E3F4A5_B667_4788_5F01_A0B1C2D3E4F5__INCLUDED_
#define WINCCMCP_APP_HISTORY_HISTORYVIEW_H__D2E3F4A5_B667_4788_5F01_A0B1C2D3E4F5__INCLUDED_

#include <QWidget>

class QTableView;

namespace winccmcp::app::history {

class SearchHistoryTableModel;


class HistoryView : public QWidget {
    Q_OBJECT

public:
    explicit HistoryView(SearchHistoryTableModel& model, QWidget* parent = nullptr);

signals:
    void rowActivated(int row);

private:
    QTableView* m_tableView;
};

} // namespace winccmcp::app::history

#endif // WINCCMCP_APP_HISTORY_HISTORYVIEW_H__D2E3F4A5_B667_4788_5F01_A0B1C2D3E4F5__INCLUDED_
