#ifndef WINCCMCP_APP_HISTORY_CHATHISTORYDIALOG_H__E3F4A5B6_C778_4899_6012_B1C2D3E4F5A6__INCLUDED_
#define WINCCMCP_APP_HISTORY_CHATHISTORYDIALOG_H__E3F4A5B6_C778_4899_6012_B1C2D3E4F5A6__INCLUDED_

#include <QDialog>

#include "core/models/ChatHistoryRecord.h"

class QLabel;
class QTextBrowser;

namespace winccmcp::app::history {

class ChatHistoryDialog : public QDialog {
    Q_OBJECT

public:
    ChatHistoryDialog(const core::models::ChatHistoryRecord& record, QWidget* parent = nullptr);

private:
    QLabel* m_queryLabel;
    QTextBrowser* m_answerView;
    QLabel* m_sourcesLabel;
};

} // namespace winccmcp::app::history

#endif // WINCCMCP_APP_HISTORY_CHATHISTORYDIALOG_H__E3F4A5B6_C778_4899_6012_B1C2D3E4F5A6__INCLUDED_
