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
