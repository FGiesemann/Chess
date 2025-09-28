/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#include "chessgui/MoveTreeWidget.h"

#include <QHeaderView>

namespace chessgui {

MoveTreeWidget::MoveTreeWidget(QWidget *parent) : QTreeView(parent) {
    setAlternatingRowColors(true);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setHeaderHidden(false);

    QPalette p = palette();
    p.setColor(QPalette::Base, QColor(245, 245, 245));
    setPalette(p);

    setFont(QFont("Inter", 10));
}

void MoveTreeWidget::setupModel(MoveTreeModel *model) {
    if (!model)
        return;
    this->setModel(model);

    QHeaderView *header = this->header();

    header->setSectionResizeMode(MoveTreeItem::MoveNumber, QHeaderView::ResizeToContents);
    header->setSectionResizeMode(MoveTreeItem::WhiteMove, QHeaderView::Stretch);
    header->setSectionResizeMode(MoveTreeItem::BlackMove, QHeaderView::Stretch);
}

} // namespace chessgui
