#include "app/history/HistoryTableItemDelegate.h"

#include <QPainter>

#include "app/history/SearchHistoryTableModel.h"

namespace winccmcp::app::history {

namespace {

QColor durationColor(qint64 ms) {
    if (ms < 1000) return QColor(0x2e, 0x7d, 0x32);   // fast - green
    if (ms < 3000) return QColor(0xe6, 0x8a, 0x00);   // medium - orange
    return QColor(0xc6, 0x28, 0x28);                  // slow - red
}

} // namespace

HistoryTableItemDelegate::HistoryTableItemDelegate(QObject* parent) : QStyledItemDelegate(parent) {
}

void HistoryTableItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    if (index.column() == SearchHistoryTableModel::DurationColumn) {
        painter->save();
        painter->setPen(durationColor(index.data(Qt::DisplayRole).toLongLong()));
        painter->drawText(option.rect.adjusted(0, 0, -6, 0), Qt::AlignRight | Qt::AlignVCenter,
                           index.data(Qt::DisplayRole).toString());
        painter->restore();
        return;
    }

    if (index.column() == SearchHistoryTableModel::QueryColumn) {
        painter->save();
        const QString elided = option.fontMetrics.elidedText(
            index.data(Qt::DisplayRole).toString(), Qt::ElideRight, option.rect.width() - 8);
        painter->drawText(option.rect.adjusted(4, 0, -4, 0), Qt::AlignLeft | Qt::AlignVCenter, elided);
        painter->restore();
        return;
    }

    QStyledItemDelegate::paint(painter, option, index);
}

} // namespace winccmcp::app::history
