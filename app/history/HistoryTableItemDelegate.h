#ifndef WINCCMCP_APP_HISTORY_HISTORYTABLEITEMDELEGATE_H__C1D2E3F4_A556_4677_4EF0_F9A0B1C2D3E4__INCLUDED_
#define WINCCMCP_APP_HISTORY_HISTORYTABLEITEMDELEGATE_H__C1D2E3F4_A556_4677_4EF0_F9A0B1C2D3E4__INCLUDED_

#include <QStyledItemDelegate>

namespace winccmcp::app::history {

class HistoryTableItemDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    explicit HistoryTableItemDelegate(QObject* parent = nullptr);

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};

} // namespace winccmcp::app::history

#endif // WINCCMCP_APP_HISTORY_HISTORYTABLEITEMDELEGATE_H__C1D2E3F4_A556_4677_4EF0_F9A0B1C2D3E4__INCLUDED_
