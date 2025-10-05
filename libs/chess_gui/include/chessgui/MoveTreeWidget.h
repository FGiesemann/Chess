/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#ifndef CHESSGUI_MOVETREEWIDGET_H
#define CHESSGUI_MOVETREEWIDGET_H

#include <QPainter>
#include <QStyledItemDelegate>
#include <QTreeView>

#include "chessgui/MoveTreeModel.h"

namespace chessgui {

class MoveTreeDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit MoveTreeDelegate(QObject *parent = nullptr);
    auto paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const -> void override;
    auto sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const -> QSize override;

    static constexpr int prefix_width = 15;
};

class MoveTreeWidget : public QTreeView {
    Q_OBJECT
public:
    explicit MoveTreeWidget(QWidget *parent = nullptr);
    ~MoveTreeWidget() override = default;

    auto setupModel(MoveTreeModel *model) -> void;
};

} // namespace chessgui

#endif
