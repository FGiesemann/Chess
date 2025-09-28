/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#include "chessgui/MoveTreeModel.h"

#include <QFont>

namespace chessgui {

MoveTreeItem::MoveTreeItem(GameNodePtr move_node, MoveTreeItem *parent_item) : m_parent{parent_item}, m_move_node(std::move(move_node)) {}

// Calculate the row number of this node in the context of its parent
auto MoveTreeItem::row() const -> int {
    if (m_parent == nullptr) {
        return 0;
    }

    auto it = std::find_if(m_parent->m_children.begin(), m_parent->m_children.end(), [this](const std::unique_ptr<MoveTreeItem> &child) { return child.get() == this; });
    if (it != m_parent->m_children.end()) {
        return std::distance(m_parent->m_children.begin(), it);
    }

    return 0;
}

auto MoveTreeItem::data(int column) const -> QVariant {
    if (m_parent == nullptr) {
        return QVariant();
    }

    const auto &currentMove = m_move_node->move();
    auto moveMaker = currentMove.piece.color;

    if (column == MoveNumber) {
        if (moveMaker == chesscore::Color::White) {
            int halfMoveCounter = 0;
            const MoveTreeItem *cursor = this;
            while (cursor != nullptr && cursor->parent() != nullptr) {
                halfMoveCounter++;
                cursor = cursor->parent();
            }

            int fullMoveNumber = (halfMoveCounter + 1) / 2;
            return QString("%1.").arg(fullMoveNumber);
        }
        return QVariant();
    }

    if (column == WhiteMove) {
        if (moveMaker == chesscore::Color::White) {
            QString moveStr = QString::fromStdString(to_string(currentMove));
            return moveStr;
        }
        return QVariant();
    }

    if (column == BlackMove) {
        if (moveMaker == chesscore::Color::Black) {
            QString moveStr = QString::fromStdString(to_string(currentMove));
            return moveStr;
        }
        return QVariant();
    }

    return QVariant();
}

auto MoveTreeItem::child(int row) -> MoveTreeItem * {
    if (row < 0 || row >= static_cast<int>(m_move_node->child_count())) {
        return nullptr;
    }
    if (row >= static_cast<int>(m_children.size())) {
        m_children.resize(m_move_node->child_count());
    }

    if (m_children[row] == nullptr) {
        GameNodePtr childNode = m_move_node->get_child(row);
        if (childNode) {
            m_children[row] = std::make_unique<MoveTreeItem>(childNode, this);
        } else {
            qWarning() << "Error: Child is null";
            return nullptr;
        }
    }

    return m_children[row].get();
}

auto MoveTreeItem::removeChild(int row) -> void {
    if (row < 0 || row >= m_children.size()) {
        return;
    }
    m_children.erase(m_children.begin() + row);
}

MoveTreeModel::MoveTreeModel(MoveTreeItem::GameNodePtr root_node, QObject *parent) : QAbstractItemModel(parent), m_root_item{std::make_unique<MoveTreeItem>(root_node)} {}

auto MoveTreeModel::handleMoveNodeAdded(const QModelIndex &parentIndex) -> bool {
    MoveTreeItem *parentItem = getItem(parentIndex);
    if (!parentItem) {
        qWarning() << "handleMoveNodeAdded: parent item not available";
        return false;
    }

    int newRow = parentItem->childCount();
    if (newRow < 0) {
        qWarning() << "handleMoveNodeAdded: parent child count invalid";
        return false;
    }

    beginInsertRows(parentIndex, newRow, newRow);
    parentItem->child(newRow);
    m_nodeItemMap.insert({parentItem->child(newRow)->move_node().get(), parentItem->child(newRow)});
    endInsertRows();
    return true;
}

auto MoveTreeModel::handleMoveNodeRemoved(const QModelIndex &index) -> bool {
    if (!index.isValid()) {
        qWarning() << "handleMoveNodeRemoved: invalid index";
        return false;
    }

    MoveTreeItem *itemToRemove = getItem(index);
    MoveTreeItem *parentItem = itemToRemove->parent();

    if (parentItem == nullptr || parentItem == m_root_item.get()) {
        qWarning() << "handleMoveNodeRemoved: cannot remove root node";
        return false;
    }

    int row = index.row();
    QModelIndex parentModelIndex = parent(index);
    beginRemoveRows(parentModelIndex, row, row);
    parentItem->removeChild(row);
    endRemoveRows();
    return true;
}

auto MoveTreeModel::getItem(const QModelIndex &index) const -> MoveTreeItem * {
    if (!index.isValid()) {
        return m_root_item.get();
    }
    auto *item = static_cast<MoveTreeItem *>(index.internalPointer());
    if (item != nullptr) {
        return item;
    }

    qWarning() << "Error: internal pointer zero for valid model index";
    return m_root_item.get();
}

auto MoveTreeModel::rowCount(const QModelIndex &parent) const -> int {
    auto *parent_item = getItem(parent);
    if (parent_item == nullptr) {
        qCritical() << "Error: could not retrieve parent from model index";
        return 0;
    }
    return parent_item->childCount();
}

auto MoveTreeModel::index(int row, int column, const QModelIndex &parent) const -> QModelIndex {
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    MoveTreeItem *parentItem = const_cast<MoveTreeModel *>(this)->getItem(parent);
    MoveTreeItem *childItem = parentItem->child(row);
    if (childItem != nullptr) {
        return createIndex(row, column, childItem);
    }
    return QModelIndex();
}

auto MoveTreeModel::parent(const QModelIndex &index) const -> QModelIndex {
    if (!index.isValid())
        return QModelIndex();

    const MoveTreeItem *childItem = getItem(index);
    if (childItem == nullptr)
        return QModelIndex();
    MoveTreeItem *parentItem = childItem->parent();
    if (parentItem == nullptr || parentItem == m_root_item.get())
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

auto MoveTreeModel::data(const QModelIndex &index, int role) const -> QVariant {
    if (!index.isValid())
        return QVariant();

    const MoveTreeItem *item = getItem(index);
    if (role != Qt::DisplayRole)
        return QVariant();
    return item->data(index.column());
}

auto MoveTreeModel::headerData(int section, Qt::Orientation orientation, int role) const -> QVariant {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case MoveTreeItem::MoveNumber:
            return QString("Zug Nr.");
        case MoveTreeItem::WhiteMove:
            return QString("Weiß");
        case MoveTreeItem::BlackMove:
            return QString("Schwarz");
        }
    }
    return QVariant();
}

auto MoveTreeModel::flags(const QModelIndex &index) const -> Qt::ItemFlags {
    if (!index.isValid())
        return Qt::NoItemFlags;
    return QAbstractItemModel::flags(index);
}

auto MoveTreeModel::createItemForNode(const std::shared_ptr<chessgame::GameNode> &node, [[maybe_unused]] MoveTreeItem *parentItem) const -> MoveTreeItem * {
    auto it = m_nodeItemMap.find(node.get());
    if (it != m_nodeItemMap.end()) {
        return it->second;
    }

    return nullptr;
}

auto MoveTreeModel::indexForNode(const MoveTreeItem::GameNodePtr &node) const -> QModelIndex {
    if (node == nullptr || node == m_root_item->move_node()) {
        return QModelIndex();
    }

    auto it = m_nodeItemMap.find(node.get());
    if (it == m_nodeItemMap.end()) {
        qWarning() << "indexForNode: Wrapper for MoveNode not found. Cannot create QModelIndex.";
        return QModelIndex();
    }

    MoveTreeItem *item = it->second;
    int row = item->row();
    return createIndex(row, 0, item);
}

} // namespace chessgui
