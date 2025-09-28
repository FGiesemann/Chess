/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#ifndef CHESSGUI_MOVETREEMODEL_H
#define CHESSGUI_MOVETREEMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include <QSharedPointer>

#include <map>

#include <chessgame/tree.h>

namespace chessgui {

class MoveTreeItem {
public:
    using GameNodePtr = std::shared_ptr<chessgame::GameNode>;

    enum Column { MoveNumber = 0, WhiteMove = 1, BlackMove = 2, ColumnCount };

    MoveTreeItem(GameNodePtr move_node, MoveTreeItem *parent_item = nullptr);

    auto row() const -> int;
    auto data(int column) const -> QVariant;
    auto parent() const -> MoveTreeItem * { return m_parent; }
    auto child(int row) -> MoveTreeItem *;
    auto childCount() const -> int { return static_cast<int>(m_children.size()); }
    auto move_node() const -> GameNodePtr { return m_move_node; }
    auto removeChild(int row) -> void;
private:
    MoveTreeItem *m_parent;
    GameNodePtr m_move_node;
    std::vector<std::unique_ptr<MoveTreeItem>> m_children;
};

class MoveTreeModel : public QAbstractItemModel {
    Q_OBJECT
public:
    explicit MoveTreeModel(MoveTreeItem::GameNodePtr root_node, QObject *parent = nullptr);

    auto columnCount(const QModelIndex & /*parent*/) const -> int override { return MoveTreeItem::ColumnCount; };
    auto rowCount(const QModelIndex &parent = QModelIndex()) const -> int override;

    auto index(int row, int column, const QModelIndex &parent = QModelIndex()) const -> QModelIndex override;
    auto parent(const QModelIndex &index) const -> QModelIndex override;

    auto data(const QModelIndex &index, int role) const -> QVariant override;
    auto headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const -> QVariant override;

    auto flags(const QModelIndex &index) const -> Qt::ItemFlags override;

    auto handleMoveNodeAdded(const QModelIndex &parentIndex) -> bool;
    auto handleMoveNodeRemoved(const QModelIndex &index) -> bool;

    auto indexForNode(const MoveTreeItem::GameNodePtr &node) const -> QModelIndex;
private:
    std::unique_ptr<MoveTreeItem> m_root_item;
    auto getItem(const QModelIndex &index) const -> MoveTreeItem *;
    mutable std::map<chessgame::GameNode *, MoveTreeItem *> m_nodeItemMap;

    auto createItemForNode(const std::shared_ptr<chessgame::GameNode> &node, MoveTreeItem *parentItem) const -> MoveTreeItem *;
};

} // namespace chessgui

#endif
