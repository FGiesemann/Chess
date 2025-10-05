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

auto MoveTreeModel::make_model_node(const NodePtr &parent, const chessgame::Cursor &cursor, int moveNumber, bool isMainline, bool is_black_variation) -> NodePtr {
    auto modelNode = std::make_shared<MoveTreeNode>();
    modelNode->parent = parent;
    if (cursor.player_color() == chesscore::Color::White) {
        modelNode->whiteCursor = cursor;
    } else {
        modelNode->blackCursor = cursor;
    }
    modelNode->moveNumber = moveNumber;
    modelNode->isMainLine = isMainline;
    modelNode->isBlackVariation = is_black_variation;
    parent->children.push_back(modelNode);
    return modelNode;
}

auto MoveTreeModel::continue_main_line(const chessgame::Cursor &black_move, const NodePtr &parent_node, int move_number, bool is_main_line) -> void {
    if (auto white_continuation = black_move.child(0); white_continuation.has_value()) {
        buildSubtree(parent_node, white_continuation.value(), move_number, is_main_line);
    }
}

auto MoveTreeModel::create_variations(const chessgame::Cursor &move, const NodePtr &parent_node, int move_number) -> void {
    for (size_t i = 1; i < move.child_count(); ++i) {
        if (auto variation = move.child(i); variation.has_value()) {
            buildSubtree(parent_node, variation.value(), move_number, false);
        }
    }
}

auto MoveTreeModel::collect_black_continuation(const chessgame::Cursor &white_move, const NodePtr &current_node, int move_number, bool is_main_line) -> void {
    if (auto black_continuation = white_move.child(0); black_continuation.has_value()) {
        current_node->blackCursor = black_continuation.value();
        continue_main_line(black_continuation.value(), current_node->parent.lock(), move_number, is_main_line);
        create_variations(black_continuation.value(), current_node, move_number);
    }
}

auto MoveTreeModel::buildSubtree(const NodePtr &parent_node, const chessgame::Cursor &move, int move_number, bool isMainLine) -> void {
    if (const auto is_white_move = move.player_color() == chesscore::Color::White; is_white_move) {
        auto current_node = make_model_node(parent_node, move, move_number, isMainLine);
        collect_black_continuation(move, current_node, move_number + 1, isMainLine);
        for (size_t i = 1; i < move.child_count(); ++i) {
            auto black_variation = move.child(i);
            if (!black_variation.has_value()) {
                continue;
            }
            auto black_variation_node = make_model_node(current_node, *black_variation, move_number, false, true);
            if (auto white_continuation = black_variation->child(0); white_continuation) {
                auto variation_continuation_node = make_model_node(current_node, *white_continuation, move_number + 1, false);
                collect_black_continuation(white_continuation.value(), variation_continuation_node, move_number + 2, false);
                create_variations(white_continuation.value(), variation_continuation_node, move_number + 1);
            }
            create_variations(black_variation.value(), black_variation_node, move_number + 1);
        }
    } else {
        auto current_node = make_model_node(parent_node, move, move_number, false, true);
        continue_main_line(move, parent_node, move_number + 1, false);
        create_variations(move, current_node, move_number + 1);
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
    } else {
        move_number = QString("%1.").arg(node->moveNumber);
    }

    return prefix + move_number;
}

} // namespace chessgui
