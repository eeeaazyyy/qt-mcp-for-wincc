#ifndef WINCCMCP_APP_HISTORY_HISTORYCONTROLLER_H__F4A5B6C7_D889_49AA_7123_C2D3E4F5A6B7__INCLUDED_
#define WINCCMCP_APP_HISTORY_HISTORYCONTROLLER_H__F4A5B6C7_D889_49AA_7123_C2D3E4F5A6B7__INCLUDED_

#include <QObject>

namespace winccmcp::app::history {

class HistoryView;
class SearchHistoryTableModel;

class HistoryController : public QObject {
    Q_OBJECT

public:
    HistoryController(HistoryView& view, SearchHistoryTableModel& model, QObject* parent = nullptr);

private:
    void onRowActivated(int row);

    HistoryView& m_view;
    SearchHistoryTableModel& m_model;
};

} // namespace winccmcp::app::history

#endif // WINCCMCP_APP_HISTORY_HISTORYCONTROLLER_H__F4A5B6C7_D889_49AA_7123_C2D3E4F5A6B7__INCLUDED_
