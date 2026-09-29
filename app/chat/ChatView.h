#ifndef WINCCMCP_APP_CHAT_CHATVIEW_H__AFB0C1D2_E334_4355_2CDE_D7E8F9A0B1C2__INCLUDED_
#define WINCCMCP_APP_CHAT_CHATVIEW_H__AFB0C1D2_E334_4355_2CDE_D7E8F9A0B1C2__INCLUDED_

#include <QWidget>

class QListView;
class QPlainTextEdit;
class QPushButton;

namespace winccmcp::app::chat {

class ChatMessageListModel;
class ChatMessageDelegate;


class ChatView : public QWidget {
    Q_OBJECT

public:
    explicit ChatView(QWidget* parent = nullptr);

    ChatMessageListModel* model() const { return m_model; }

    void setInputEnabled(bool enabled);

signals:
    void messageSubmitted(const QString& text);

protected:
    void changeEvent(QEvent* event) override;

private:
    void retranslateUi();
    void submitInput();

    ChatMessageListModel* m_model;
    ChatMessageDelegate* m_delegate;
    QListView* m_listView;
    QPlainTextEdit* m_input;
    QPushButton* m_sendButton;
};

} // namespace winccmcp::app::chat

#endif // WINCCMCP_APP_CHAT_CHATVIEW_H__AFB0C1D2_E334_4355_2CDE_D7E8F9A0B1C2__INCLUDED_
