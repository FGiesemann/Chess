/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#include "chessgui/MoveTreeModel.h"

#include <chessgame/san.h>

#include <utility>

namespace chessgui {

MoveTreeModel::MoveTreeModel(QObject *parent) : QAbstractItemModel(parent), m_root(std::make_shared<MoveTreeNode>()) {}

MoveTreeModel::MoveTreeModel(GamePtr game, QObject *parent) : QAbstractItemModel(parent), m_game(std::move(game)), m_root(std::make_shared<MoveTreeNode>()) {
    buildTree();
}

auto MoveTreeModel::setGame(GamePtr game) -> void {
    beginResetModel();
    m_game = std::move(game);
    buildTree();
    endResetModel();
    emit gameChanged();
}

auto MoveTreeModel::rebuildTree() -> void {
    beginResetModel();
    buildTree();
    endResetModel();
}

auto MoveTreeModel::onMoveAdded(const chessgame::Cursor &parentCursor, size_t childIndex) -> void {
    if (!m_game) {
        return;
    }

    auto parentNodeId = parentCursor.node_id();
    auto parentModelNode = modelNodeByCursor(parentCursor);
    if (!parentModelNode) {
        rebuildTree();
        return;
    }

    const auto parent_is_white = parentModelNode->whiteCursor && parentModelNode->whiteCursor->node_id() == parentNodeId;
    auto childCursorOpt = parentCursor.child(childIndex);
    if (!childCursorOpt) {
        rebuildTree();
        return;
    }
    const auto &newCursor = *childCursorOpt;
    if (parent_is_white) {
        handleMoveAddedToWhiteNode(parentModelNode, newCursor, childIndex);
    } else if (parentModelNode->blackCursor && parentModelNode->blackCursor->node()->id() == parentNodeId) {
        handleMoveAddedToBlackNode(parentModelNode, newCursor, childIndex);
    } else {
        // This shouldn't happen
        rebuildTree();
    }
}

auto MoveTreeModel::handleMoveAddedToWhiteNode(const NodePtr &modelNode, const chessgame::Cursor &newCursor, size_t childIndex) -> void {
    if (childIndex == 0) {
        modelNode->blackCursor = newCursor;
        auto idx = indexFromModelNode(modelNode);
        emit dataChanged(idx, index(idx.row(), ColumnCount - 1, idx.parent()));
    } else {
        auto newModelNode = std::make_shared<MoveTreeNode>();
        newModelNode->parent = modelNode;
        newModelNode->blackCursor = newCursor;
        newModelNode->moveNumber = modelNode->moveNumber;
        newModelNode->isMainLine = false;
        newModelNode->isBlackVariation = true;

        auto parentIdx = indexFromModelNode(modelNode);
        int insertRow = static_cast<int>(modelNode->children.size());

        beginInsertRows(parentIdx, insertRow, insertRow);
        modelNode->children.push_back(newModelNode);
        endInsertRows();
    }
}

auto MoveTreeModel::handleMoveAddedToBlackNode(const NodePtr &modelNode, const chessgame::Cursor &newCursor, size_t childIndex) -> void {
    auto newModelNode = std::make_shared<MoveTreeNode>();
    newModelNode->whiteCursor = newCursor;
    newModelNode->moveNumber = modelNode->moveNumber + 1;

    if (childIndex == 0) {
        auto parentNode = modelNode->parent.lock();
        if (!parentNode) {
            // This shouldn't happen, but handle gracefully
            rebuildTree();
            return;
        }

        newModelNode->parent = parentNode;
        newModelNode->isMainLine = modelNode->isMainLine;

        auto parentIdx = indexFromModelNode(parentNode);
        size_t insertPos = 0;
        for (size_t i = 0; i < parentNode->children.size(); ++i) {
            if (parentNode->children[i] == modelNode) {
                insertPos = i + 1;
                break;
            }
        }

        beginInsertRows(parentIdx, static_cast<int>(insertPos), static_cast<int>(insertPos));
        parentNode->children.insert(parentNode->children.begin() + static_cast<int>(insertPos), newModelNode);
        endInsertRows();
    } else {
        newModelNode->parent = modelNode;
        newModelNode->isMainLine = false;

        auto parentIdx = indexFromModelNode(modelNode);
        int insertRow = static_cast<int>(modelNode->children.size());

        beginInsertRows(parentIdx, insertRow, insertRow);
        modelNode->children.push_back(newModelNode);
        endInsertRows();
    }
}

auto MoveTreeModel::onNodeDataChanged(const chessgame::Cursor &cursor) -> void {
    auto modelNode = modelNodeByCursor(cursor);

    if (!modelNode) {
        return;
    }

    auto idx = indexFromModelNode(modelNode);
    if (idx.isValid()) {
        emit dataChanged(idx, index(idx.row(), ColumnCount - 1, idx.parent()));
    }
}

auto MoveTreeModel::searchForCursor(const chessgame::Cursor &cursor, const NodePtr &node) -> NodePtr {
    if (node == nullptr) {
        return nullptr;
    }
    if ((node->whiteCursor && node->whiteCursor->node_id() == cursor.node_id()) || (node->blackCursor && node->blackCursor->node_id() == cursor.node_id())) {
        return node;
    }
    for (const auto &child : node->children) {
        if (auto found = searchForCursor(cursor, child)) {
            return found;
        }
    }
    return nullptr;
}

auto MoveTreeModel::modelNodeByCursor(const chessgame::Cursor &cursor) const -> NodePtr {
    if (!cursor.node()) {
        return nullptr;
    }
    return searchForCursor(cursor, m_root);
}

auto MoveTreeModel::indexFromModelNode(const NodePtr &node, int column) const -> QModelIndex {
    if (!node || node == m_root) {
        return {};
    }

    auto parent = node->parent.lock();
    if (!parent) {
        return {};
    }
    for (size_t i = 0; i < parent->children.size(); ++i) {
        if (parent->children[i] == node) {
            return createIndex(static_cast<int>(i), column, node.get());
        }
    }

    return {};
}

auto MoveTreeModel::buildTree() -> void {
    m_root = std::make_shared<MoveTreeNode>();

    if (!m_game) {
        return;
    }

    auto cursor = m_game->cursor();

    int moveNumber = 1;

    if (cursor.child_count() > 0) {
        auto firstChild = cursor.child(0);
        if (firstChild) {
            buildSubtree(m_root, *firstChild, moveNumber, true);
        }
    }
}

auto MoveTreeModel::buildSubtree(const NodePtr &parentModelNode, const chessgame::Cursor &cursor, int moveNumber, bool isMainLine) -> void {
    auto pos = cursor.position();
    bool isWhiteMove = (pos.side_to_move() == chesscore::Color::Black);

    NodePtr currentModelNode;

    if (isWhiteMove) {
        currentModelNode = std::make_shared<MoveTreeNode>();
        currentModelNode->parent = parentModelNode;
        currentModelNode->whiteCursor = cursor;
        currentModelNode->moveNumber = moveNumber;
        currentModelNode->isMainLine = isMainLine;

        parentModelNode->children.push_back(currentModelNode);

        if (cursor.child_count() > 0) {
            auto blackCursorOpt = cursor.child(0);
            if (blackCursorOpt) {
                currentModelNode->blackCursor = *blackCursorOpt;

                if (blackCursorOpt->child_count() > 0) {
                    auto nextCursorOpt = blackCursorOpt->child(0);
                    if (nextCursorOpt) {
                        buildSubtree(parentModelNode, *nextCursorOpt, moveNumber + 1, isMainLine);
                    }
                }

                for (size_t i = 1; i < blackCursorOpt->child_count(); ++i) {
                    auto varCursorOpt = blackCursorOpt->child(i);
                    if (varCursorOpt) {
                        buildSubtree(currentModelNode, *varCursorOpt, moveNumber + 1, false);
                    }
                }
            }
        }

        for (size_t i = 1; i < cursor.child_count(); ++i) {
            auto varCursorOpt = cursor.child(i);
            if (!varCursorOpt) {
                continue;
            }

            auto variationNode = std::make_shared<MoveTreeNode>();
            variationNode->parent = currentModelNode;
            variationNode->blackCursor = *varCursorOpt;
            variationNode->moveNumber = moveNumber;
            variationNode->isMainLine = false;
            variationNode->isBlackVariation = true;

            currentModelNode->children.push_back(variationNode);

            if (varCursorOpt->child_count() > 0) {
                auto nextCursorOpt = varCursorOpt->child(0);
                if (nextCursorOpt) {
                    buildSubtree(variationNode, *nextCursorOpt, moveNumber + 1, false);
                }
            }

            for (size_t j = 1; j < varCursorOpt->child_count(); ++j) {
                auto subVarCursorOpt = varCursorOpt->child(j);
                if (subVarCursorOpt) {
                    buildSubtree(variationNode, *subVarCursorOpt, moveNumber + 1, false);
                }
            }
        }

    } else {
        currentModelNode = std::make_shared<MoveTreeNode>();
        currentModelNode->parent = parentModelNode;
        currentModelNode->blackCursor = cursor;
        currentModelNode->moveNumber = moveNumber;
        currentModelNode->isMainLine = false;
        currentModelNode->isBlackVariation = true;

        parentModelNode->children.push_back(currentModelNode);

        if (cursor.child_count() > 0) {
            auto nextCursorOpt = cursor.child(0);
            if (nextCursorOpt) {
                buildSubtree(parentModelNode, *nextCursorOpt, moveNumber + 1, false);
            }
        }

        for (size_t i = 1; i < cursor.child_count(); ++i) {
            auto varCursorOpt = cursor.child(i);
            if (varCursorOpt) {
                buildSubtree(currentModelNode, *varCursorOpt, moveNumber + 1, false);
            }
        }
    }
}

auto MoveTreeModel::index(int row, int column, const QModelIndex &parent) const -> QModelIndex {
    if (!hasIndex(row, column, parent)) {
        return {};
    }

    auto parentNode = parent.isValid() ? modelNodeFromIndex(parent) : m_root;

    if (!parentNode || std::cmp_greater_equal(row, parentNode->children.size())) {
        return {};
    }

    auto childNode = parentNode->children[row];
    return createIndex(row, column, childNode.get());
}

auto MoveTreeModel::parent(const QModelIndex &child) const -> QModelIndex {
    if (!child.isValid()) {
        return {};
    }

    auto childNode = modelNodeFromIndex(child);
    if (!childNode) {
        return {};
    }

    auto parentNode = childNode->parent.lock();
    if (!parentNode || parentNode == m_root) {
        return {};
    }

    auto grandParentNode = parentNode->parent.lock();
    if (!grandParentNode) {
        return {};
    }

    for (size_t i = 0; i < grandParentNode->children.size(); ++i) {
        if (grandParentNode->children[i] == parentNode) {
            return createIndex(static_cast<int>(i), 0, parentNode.get());
        }
    }

    return {};
}

auto MoveTreeModel::rowCount(const QModelIndex &parent) const -> int {
    if (parent.column() > 0) {
        return 0;
    }

    auto parentNode = parent.isValid() ? modelNodeFromIndex(parent) : m_root;
    return parentNode ? static_cast<int>(parentNode->children.size()) : 0;
}

auto MoveTreeModel::columnCount(const QModelIndex &parent) const -> int {
    Q_UNUSED(parent)
    return ColumnCount;
}

auto MoveTreeModel::data(const QModelIndex &index, int role) const -> QVariant {
    if (!index.isValid()) {
        return {};
    }

    auto node = modelNodeFromIndex(index);
    if (!node) {
        return {};
    }

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case MoveNumberColumn:
            return moveNumberText(node, index.column());
        case WhiteMoveColumn:
            return node->whiteCursor ? moveText(*node->whiteCursor) : QString();
        case BlackMoveColumn:
            return node->blackCursor ? moveText(*node->blackCursor) : QString();
        default:
            return {};
        }
    }

    if (index.column() == 0) {
        switch (role) {
        case HasCommentRole:
            return (node->whiteCursor && !node->whiteCursor->comment().empty()) || (node->blackCursor && !node->blackCursor->comment().empty());

        case HasPremoveCommentRole:
            return (node->whiteCursor && !node->whiteCursor->premove_comment().empty()) || (node->blackCursor && !node->blackCursor->premove_comment().empty());

        case HasVariationsRole:
            return node->children.size() > 1;

        case IsMainLineRole:
            return node->isMainLine;

        case MoveNumberRole:
            return node->moveNumber;

        case IsWhiteVariationRole:
            return node->isWhiteVariation;

        case IsBlackVariationRole:
            return node->isBlackVariation;

        case HasNagsRole:
            return (node->whiteCursor && !node->whiteCursor->nags().empty()) || (node->blackCursor && !node->blackCursor->nags().empty());

        case NodeIdRole:
            if (node->whiteCursor) {
                return QVariant::fromValue(node->whiteCursor->node_id());
            } else if (node->blackCursor) {
                return QVariant::fromValue(node->blackCursor->node_id());
            }
            return {};

        default:
            break;
        }
    }

    return {};
}

auto MoveTreeModel::headerData(int section, Qt::Orientation orientation, int role) const -> QVariant {
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
        return {};
    }

    switch (section) {
    case MoveNumberColumn:
        return tr("Move");
    case WhiteMoveColumn:
        return tr("White");
    case BlackMoveColumn:
        return tr("Black");
    default:
        return {};
    }
}

auto MoveTreeModel::flags(const QModelIndex &index) const -> Qt::ItemFlags {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

auto MoveTreeModel::cursorFromIndex(const QModelIndex &index) const -> std::optional<chessgame::Cursor> {
    if (!index.isValid()) {
        return std::nullopt;
    }

    auto node = modelNodeFromIndex(index);
    if (!node) {
        return std::nullopt;
    }

    if (index.column() == WhiteMoveColumn && node->whiteCursor) {
        return node->whiteCursor;
    }
    if (index.column() == BlackMoveColumn && node->blackCursor) {
        return node->blackCursor;
    }
    if (node->whiteCursor) {
        return node->whiteCursor;
    }
    return node->blackCursor;
}

auto MoveTreeModel::modelNodeFromIndex(const QModelIndex &index) const -> NodePtr {
    if (!index.isValid()) {
        return nullptr;
    }

    auto *ptr = static_cast<MoveTreeNode *>(index.internalPointer());
    if (ptr == nullptr) {
        return nullptr;
    }

    std::function<NodePtr(const NodePtr &, MoveTreeNode *)> findNode;
    findNode = [&](const NodePtr &current, MoveTreeNode *target) -> NodePtr {
        if (current.get() == target) {
            return current;
        }
        for (const auto &child : current->children) {
            if (auto found = findNode(child, target)) {
                return found;
            }
        }
        return nullptr;
    };

    return findNode(m_root, ptr);
}

auto MoveTreeModel::moveText(const chessgame::Cursor &cursor) -> QString {
    const auto parent = cursor.parent();
    if (!parent) {
        return QString{"No Parent"};
    }
    const auto position = cursor.position();
    const auto check_state = position.check_state();
    QString check_marker = QString::fromStdString(check_state_symbol(check_state));
    const auto parent_position = parent->position();
    const auto all_moves = parent_position.all_legal_moves();
    const auto &move = cursor.move();
    const auto opt_san = chessgame::generate_san_move(move, all_moves);
    if (opt_san.has_value()) {
        return QString::fromStdString(opt_san.value().san_string) + check_marker;
    }
    return QString{"No SAN move"};
}

auto MoveTreeModel::moveNumberText(const NodePtr &node, int column) -> QString {
    Q_UNUSED(column)

    if (!node) {
        return {};
    }

    if (node->isBlackVariation) {
        return QString("%1...").arg(node->moveNumber);
    }
    if (node->isWhiteVariation) {
        return QString("%1.").arg(node->moveNumber);
    }
    return QString("%1.").arg(node->moveNumber);
}

} // namespace chessgui
