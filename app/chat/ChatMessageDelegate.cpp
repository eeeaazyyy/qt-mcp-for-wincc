#include "app/chat/ChatMessageDelegate.h"

#include <QAbstractTextDocumentLayout>
#include <QPainter>
#include <QPainterPath>
#include <QTextDocument>

#include <cmath>

#include "app/chat/ChatMessageListModel.h"

namespace winccmcp::app::chat {

namespace {

QColor bubbleColor(ChatMessageKind kind, bool isDark) {
    switch (kind) {
        case ChatMessageKind::User:
            return isDark ? QColor(0x2d, 0x5a, 0x8a) : QColor(0xdc, 0xf0, 0xff);
        case ChatMessageKind::Assistant:
            return isDark ? QColor(0x3a, 0x3f, 0x47) : QColor(0xf0, 0xf0, 0xf0);
        case ChatMessageKind::SystemNotice:
            return isDark ? QColor(0x5a, 0x3a, 0x2d) : QColor(0xff, 0xf0, 0xdc);
    }
    return QColor(Qt::gray);
}

} // namespace

ChatMessageDelegate::ChatMessageDelegate(QObject* parent) : QStyledItemDelegate(parent) {
}

int ChatMessageDelegate::maxBubbleWidth(const QStyleOptionViewItem& option) const {
    return option.rect.width() * kBubbleMaxWidthRatio / 100;
}

QSizeF ChatMessageDelegate::bubbleTextSize(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QTextDocument doc;
    doc.setDefaultFont(option.font);
    doc.setPlainText(index.data(Qt::DisplayRole).toString());
    doc.setTextWidth(maxBubbleWidth(option) - 2 * kPadding);

    return QSizeF(qMin(doc.idealWidth(),
                static_cast<qreal>(maxBubbleWidth(option) - 2 * kPadding)), doc.size().height());
}

void ChatMessageDelegate::paint(QPainter* painter,
                const QStyleOptionViewItem& option, const QModelIndex& index) const {
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    const auto kind = static_cast<ChatMessageKind>(index.data(ChatMessageListModel::KindRole).toInt());
    const bool alignRight = (kind == ChatMessageKind::User);
    const bool isDark = option.palette.window().color().lightness() < 128;

    const QSizeF textSize = bubbleTextSize(option, index);
    const int bubbleWidth = static_cast<int>(std::ceil(textSize.width())) + 2 * kPadding;
    const int bubbleHeight = static_cast<int>(std::ceil(textSize.height())) + 2 * kPadding;

    const int x = alignRight ? option.rect.right() - bubbleWidth - kMargin : option.rect.left() + kMargin;
    const QRect bubbleRect(x, option.rect.top() + kMargin / 2, bubbleWidth, bubbleHeight);

    QPainterPath path;
    path.addRoundedRect(bubbleRect, 10, 10);
    painter->fillPath(path, bubbleColor(kind, isDark));

    QTextDocument doc;
    doc.setDefaultFont(option.font);
    doc.setPlainText(index.data(Qt::DisplayRole).toString());
    doc.setTextWidth(bubbleWidth - 2 * kPadding);

    QAbstractTextDocumentLayout::PaintContext context;
    context.palette.setColor(QPalette::Text, isDark ? QColor(0xf0, 0xf0, 0xf0) : QColor(0x1a, 0x1a, 0x1a));

    painter->translate(bubbleRect.left() + kPadding, bubbleRect.top() + kPadding);
    doc.documentLayout()->draw(painter, context);

    painter->restore();
}

QSize ChatMessageDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
    const QSizeF textSize = bubbleTextSize(option, index);
    return QSize(option.rect.width(), static_cast<int>(textSize.height()) + 2 * kPadding + kMargin);
}

} // namespace winccmcp::app::chat
