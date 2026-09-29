#ifndef WINCCMCP_APP_CHAT_CHATMESSAGEDELEGATE_H__9EAFB0C1_D223_4244_1BCD_C6D7E8F9A0B1__INCLUDED_
#define WINCCMCP_APP_CHAT_CHATMESSAGEDELEGATE_H__9EAFB0C1_D223_4244_1BCD_C6D7E8F9A0B1__INCLUDED_

#include <QStyledItemDelegate>

namespace winccmcp::app::chat {

class ChatMessageDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    explicit ChatMessageDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    static constexpr int kBubbleMaxWidthRatio = 70; // % of the list's width
    static constexpr int kMargin = 8;
    static constexpr int kPadding = 10;

    QSizeF bubbleTextSize(const QStyleOptionViewItem& option, const QModelIndex& index) const;
    int maxBubbleWidth(const QStyleOptionViewItem& option) const;
};

} // namespace winccmcp::app::chat

#endif // WINCCMCP_APP_CHAT_CHATMESSAGEDELEGATE_H__9EAFB0C1_D223_4244_1BCD_C6D7E8F9A0B1__INCLUDED_
