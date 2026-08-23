/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#ifndef CHESSGUI_MOVETREEWIDGET_H
#define CHESSGUI_MOVETREEWIDGET_H

#include <QPainter>
#include <QStyledItemDelegate>
#include <QTreeView>

#include "chess_gui/MoveTreeModel.h"

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

    auto setModel(MoveTreeModel *model) -> void;
    auto model() const -> MoveTreeModel * { return m_model; }
signals:
    auto moveClicked(chessgame::Cursor cursor) -> void;
    auto moveDoubleClicked(chessgame::Cursor cursor) -> void;
    auto moveSelected(chessgame::Cursor cursor) -> void;
    auto selectionCleared() -> void;
private slots:
    auto onItemClicked(const QModelIndex &index) -> void;
    auto onItemDoubleClicked(const QModelIndex &index) -> void;
    auto onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) -> void;
private:
    MoveTreeModel *m_model{};

    auto setupUI() -> void;
};

} // namespace chessgui

#endif
