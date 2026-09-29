#ifndef WINCCMCP_APP_CHAT_CHATMESSAGELISTMODEL_H__8D9EAFB0_C112_4133_0ABC_B5C6D7E8F9A0__INCLUDED_
#define WINCCMCP_APP_CHAT_CHATMESSAGELISTMODEL_H__8D9EAFB0_C112_4133_0ABC_B5C6D7E8F9A0__INCLUDED_

#include <QAbstractListModel>
#include <QVector>

#include "app/chat/ChatMessage.h"

namespace winccmcp::app::chat {

class ChatMessageListModel : public QAbstractListModel {
    Q_OBJECT

public:
    enum Role {
        KindRole = Qt::UserRole + 1,
        TimestampRole,
    };

    explicit ChatMessageListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void append(const ChatMessage& message);
    void clear();

private:
    QVector<ChatMessage> m_messages;
};

} // namespace winccmcp::app::chat

#endif // WINCCMCP_APP_CHAT_CHATMESSAGELISTMODEL_H__8D9EAFB0_C112_4133_0ABC_B5C6D7E8F9A0__INCLUDED_
