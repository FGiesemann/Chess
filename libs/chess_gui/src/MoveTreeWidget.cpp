/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#include "chessgui/MoveTreeWidget.h"

#include <QApplication>
#include <QHeaderView>

namespace chessgui {

MoveTreeDelegate::MoveTreeDelegate(QObject *parent) : QStyledItemDelegate(parent) {}

void MoveTreeDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (index.column() != MoveTreeModel::MoveNumberColumn) {
        QStyledItemDelegate::paint(painter, option, index);
        return;
    }

    auto opt = option;
    initStyleOption(&opt, index);
    auto originalText = opt.text;
    opt.text.clear();
    auto *style = (opt.widget != nullptr) ? opt.widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

    auto textRect = style->subElementRect(QStyle::SE_ItemViewItemText, &opt, opt.widget);
    QString prefix;
    auto number = originalText;
    bool hasPrefix = false;

    if (originalText.startsWith("▶")) {
        prefix = originalText.left(1);
        number = originalText.mid(1).trimmed();
        hasPrefix = true;
    }

    painter->save();
    painter->setFont(opt.font);
    painter->setPen(opt.palette.color(opt.state & QStyle::State_Selected ? QPalette::HighlightedText : QPalette::Text));

    if (hasPrefix) {
        QRect prefixRect = textRect;
        prefixRect.setWidth(prefix_width);
        painter->drawText(prefixRect, Qt::AlignLeft | Qt::AlignVCenter, prefix);
    }
    QRect numberRect = textRect;
    numberRect.setLeft(numberRect.left() + prefix_width);
    painter->drawText(numberRect, Qt::AlignLeft | Qt::AlignVCenter, number);
    painter->restore();
}

auto MoveTreeDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const -> QSize {
    QSize size = QStyledItemDelegate::sizeHint(option, index);

    if (index.column() == MoveTreeModel::MoveNumberColumn) {
        size.setWidth(size.width() + prefix_width);
    }

    return size;
}

MoveTreeWidget::MoveTreeWidget(QWidget *parent) : QTreeView(parent) {
    setItemDelegate(new MoveTreeDelegate(this));
    setAlternatingRowColors(true);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectItems);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setHeaderHidden(false);

    QPalette p = palette();
    p.setColor(QPalette::Base, QColor(245, 245, 245));
    setPalette(p);

    setFont(QFont("Inter", 10));
}

auto MoveTreeWidget::setupModel(MoveTreeModel *model) -> void {
    if (model == nullptr)
        return;
    this->setModel(model);

    QHeaderView *header = this->header();
    header->setSectionResizeMode(MoveTreeModel::MoveNumberColumn, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(MoveTreeModel::WhiteMoveColumn, QHeaderView::Stretch);
    header->setSectionResizeMode(MoveTreeModel::BlackMoveColumn, QHeaderView::Stretch);
}

} // namespace chessgui
