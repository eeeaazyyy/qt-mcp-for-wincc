#include "app/chat/ChatView.h"

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QListView>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "app/chat/ChatMessageDelegate.h"
#include "app/chat/ChatMessageListModel.h"

namespace winccmcp::app::chat {

namespace {

class ChatInputEdit : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit ChatInputEdit(QWidget* parent = nullptr) : QPlainTextEdit(parent) {
        setFixedHeight(72);
    }

signals:
    void submitRequested();

protected:
    void keyPressEvent(QKeyEvent* event) override {
        const bool isEnter = (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter);
        if (isEnter && !(event->modifiers() & Qt::ShiftModifier)) {
            emit submitRequested();
            return;
        }
        QPlainTextEdit::keyPressEvent(event);
    }
};

} // namespace

ChatView::ChatView(QWidget* parent) : QWidget(parent) {
    m_model = new ChatMessageListModel(this);
    m_delegate = new ChatMessageDelegate(this);

    m_listView = new QListView(this);
    m_listView->setModel(m_model);
    m_listView->setItemDelegate(m_delegate);
    m_listView->setResizeMode(QListView::Adjust);
    m_listView->setSelectionMode(QAbstractItemView::NoSelection);
    m_listView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_listView->setSpacing(2);

    auto* input = new ChatInputEdit(this);
    m_input = input;

    m_sendButton = new QPushButton(this);
    connect(m_sendButton, &QPushButton::clicked, this, &ChatView::submitInput);
    connect(input, &ChatInputEdit::submitRequested, this, &ChatView::submitInput);

    auto* inputRow = new QHBoxLayout;
    inputRow->addWidget(m_input, 1);
    inputRow->addWidget(m_sendButton);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(m_listView, 1);
    layout->addLayout(inputRow);

    connect(m_model, &ChatMessageListModel::rowsInserted, m_listView, [this] { m_listView->doItemsLayout(); });
    connect(m_model, &ChatMessageListModel::rowsInserted, m_listView, &QListView::scrollToBottom);

    retranslateUi();
}

void ChatView::setInputEnabled(bool enabled) {
    m_input->setEnabled(enabled);
    m_sendButton->setEnabled(enabled);
}

void ChatView::submitInput() {
    const QString text = m_input->toPlainText().trimmed();
    if (text.isEmpty()) {
        return;
    }
    m_input->clear();
    emit messageSubmitted(text);
}

void ChatView::changeEvent(QEvent* event) {
    QWidget::changeEvent(event);
    if (event->type() == QEvent::LanguageChange) {
        retranslateUi();
    }
}

void ChatView::retranslateUi() {
    m_sendButton->setText(tr("Send"));
    m_input->setPlaceholderText(tr("Ask a question about the WinCC OA documentation..."));
}

} // namespace winccmcp::app::chat

#include "ChatView.moc"
