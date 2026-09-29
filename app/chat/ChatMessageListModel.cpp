#include "app/chat/ChatMessageListModel.h"

namespace winccmcp::app::chat {

ChatMessageListModel::ChatMessageListModel(QObject* parent) : QAbstractListModel(parent) {
}

int ChatMessageListModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : m_messages.size();
}

QVariant ChatMessageListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_messages.size()) {
        return {};
    }

    const ChatMessage& message = m_messages.at(index.row());
    switch (role) {
        case Qt::DisplayRole:
            return message.text;
        case KindRole:
            return static_cast<int>(message.kind);
        case TimestampRole:
            return message.timestamp;
        default:
            return {};
    }
}

QHash<int, QByteArray> ChatMessageListModel::roleNames() const {
    auto roles = QAbstractListModel::roleNames();
    roles[KindRole] = "kind";
    roles[TimestampRole] = "timestamp";
    return roles;
}

void ChatMessageListModel::append(const ChatMessage& message) {
    const int row = m_messages.size();
    beginInsertRows(QModelIndex(), row, row);
    m_messages.push_back(message);
    endInsertRows();
}

void ChatMessageListModel::clear() {
    if (m_messages.isEmpty()) {
        return;
    }
    beginResetModel();
    m_messages.clear();
    endResetModel();
}

} // namespace winccmcp::app::chat
