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

auto MoveTreeModel::buildTree() -> void {
    m_root = std::make_shared<MoveTreeNode>();

    if (!m_game) {
        return;
    }

    int moveNumber = 1;
    auto firstChild = m_game->cursor().child(0);
    if (firstChild.has_value()) {
        buildSubtree(m_root, firstChild.value(), moveNumber, true);
    }
}

auto MoveTreeModel::make_model_node(const NodePtr &parent, const chessgame::Cursor &cursor, int moveNumber, bool isMainline) -> NodePtr {
    auto modelNode = std::make_shared<MoveTreeNode>();
    modelNode->parent = parent;
    if (cursor.player_color() == chesscore::Color::White) {
        modelNode->whiteCursor = cursor;
    } else {
        modelNode->blackCursor = cursor;
    }
    modelNode->moveNumber = moveNumber;
    modelNode->isMainLine = isMainline;
    return modelNode;
}

auto MoveTreeModel::buildSubtree(const NodePtr &parentModelNode, const chessgame::Cursor &cursor, int moveNumber, bool isMainLine) -> void {
    bool isWhiteMove = cursor.player_color() == chesscore::Color::White;

    NodePtr currentModelNode;

    if (isWhiteMove) {
        currentModelNode = make_model_node(parentModelNode, cursor, moveNumber, isMainLine);
        parentModelNode->children.push_back(currentModelNode);

        auto blackCursorOpt = cursor.child(0);
        if (blackCursorOpt) {
            currentModelNode->blackCursor = *blackCursorOpt;

            auto nextCursorOpt = blackCursorOpt->child(0);
            if (nextCursorOpt) {
                buildSubtree(parentModelNode, *nextCursorOpt, moveNumber + 1, isMainLine);
            }

            for (size_t i = 1; i < blackCursorOpt->child_count(); ++i) {
                auto varCursorOpt = blackCursorOpt->child(i);
                if (varCursorOpt) {
                    buildSubtree(currentModelNode, *varCursorOpt, moveNumber + 1, false);
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

            NodePtr whiteContinuationNode;
            if (varCursorOpt->child_count() > 0) {
                auto nextCursorOpt = varCursorOpt->child(0);
                if (nextCursorOpt) {
                    // Create the white continuation on the same level
                    whiteContinuationNode = std::make_shared<MoveTreeNode>();
                    whiteContinuationNode->parent = currentModelNode;
                    whiteContinuationNode->whiteCursor = *nextCursorOpt;
                    whiteContinuationNode->moveNumber = moveNumber + 1;
                    whiteContinuationNode->isMainLine = false;

                    currentModelNode->children.push_back(whiteContinuationNode);

                    // Add black move to the same node if it exists
                    if (nextCursorOpt->child_count() > 0) {
                        auto blackAfterOpt = nextCursorOpt->child(0);
                        if (blackAfterOpt) {
                            whiteContinuationNode->blackCursor = *blackAfterOpt;

                            // Continue recursively from this point on the same level
                            if (blackAfterOpt->child_count() > 0) {
                                auto furtherCursorOpt = blackAfterOpt->child(0);
                                if (furtherCursorOpt) {
                                    buildSubtree(currentModelNode, *furtherCursorOpt, moveNumber + 2, false);
                                }
                            }

                            // Handle black variations
                            for (size_t k = 1; k < blackAfterOpt->child_count(); ++k) {
                                auto blackVarOpt = blackAfterOpt->child(k);
                                if (blackVarOpt) {
                                    buildSubtree(whiteContinuationNode, *blackVarOpt, moveNumber + 2, false);
                                }
                            }
                        }
                    }

                    // Handle white variations of the continuation - these are children of whiteContinuationNode
                    for (size_t j = 1; j < nextCursorOpt->child_count(); ++j) {
                        auto whiteVarOpt = nextCursorOpt->child(j);
                        if (whiteVarOpt) {
                            buildSubtree(whiteContinuationNode, *whiteVarOpt, moveNumber + 1, false);
                        }
                    }
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
        currentModelNode = make_model_node(parentModelNode, cursor, moveNumber, false);
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

auto MoveTreeModel::onMoveAdded(const chessgame::Cursor &parentCursor, size_t childIndex) -> void {
    if (m_game == nullptr) {
        return;
    }

    auto parentModelNode = modelNodeByCursor(parentCursor);
    if (parentModelNode == nullptr) {
        rebuildTree();
        return;
    }

    auto childCursorOpt = parentCursor.child(childIndex);
    if (!childCursorOpt.has_value()) {
        rebuildTree();
        return;
    }
    const auto &newCursor = *childCursorOpt;
    const auto parent_is_white = parentModelNode->whiteCursor && parentModelNode->whiteCursor == parentCursor;
    if (parent_is_white) {
        handleMoveAddedToWhiteNode(parentModelNode, newCursor, childIndex);
    } else {
        handleMoveAddedToBlackNode(parentModelNode, newCursor, childIndex);
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
    if ((node->whiteCursor && node->whiteCursor == cursor) || (node->blackCursor && node->blackCursor == cursor)) {
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
            // case Qt::TextAlignmentRole:
            //     return Qt::AlignRight;

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

    QString prefix{};
    const auto cursor = (node->whiteCursor) ? node->whiteCursor : node->blackCursor;
    if (cursor && cursor->variation_number() > 0) {
        prefix = QString{"▶"};
    }

    QString move_number{};
    if (node->isBlackVariation) {
        move_number = QString("%1...").arg(node->moveNumber);
    }
    move_number = QString("%1.").arg(node->moveNumber);

    return prefix + move_number;
}

} // namespace chessgui
