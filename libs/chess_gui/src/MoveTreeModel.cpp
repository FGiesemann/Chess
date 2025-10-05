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
    build_tree();
}

auto MoveTreeModel::setGame(GamePtr game) -> void {
    beginResetModel();
    m_game = std::move(game);
    build_tree();
    endResetModel();
    emit gameChanged();
}

auto MoveTreeModel::rebuildTree() -> void {
    beginResetModel();
    build_tree();
    endResetModel();
}

auto MoveTreeModel::build_tree() -> void {
    m_root = std::make_shared<MoveTreeNode>();

    if (!m_game) {
        return;
    }

    int moveNumber = 1;
auto MoveTreeModel::build_subtree(const NodePtr &parent_node, const chessgame::Cursor &move, int move_number, bool is_main_line) -> void {
    if (const auto is_white_move = move.player_color() == chesscore::Color::White; is_white_move) {
        auto move_node = make_model_node(parent_node, move, move_number, is_main_line);
        create_variations(move, move_node, move_number);
        collect_black_continuation(move, move_node, move_number + 1, is_main_line);
    } else {
        auto move_node = make_model_node(parent_node, move, move_number, false, true);
        create_variations(move, move_node, move_number + 1);
        continue_white_main_line(move, parent_node, move_number + 1, false);
    }
}

auto MoveTreeModel::make_model_node(const NodePtr &parent, const chessgame::Cursor &cursor, int move_number, bool is_main_line, bool is_black_variation) -> NodePtr {
    auto model_node = std::make_shared<MoveTreeNode>();
    model_node->parent = parent;
    if (cursor.player_color() == chesscore::Color::White) {
        model_node->white_cursor = cursor;
    } else {
        model_node->black_cursor = cursor;
    }
    model_node->move_number = move_number;
    model_node->is_main_line = is_main_line;
    model_node->is_black_variation = is_black_variation;
    parent->children.push_back(model_node);
    return model_node;
}

auto MoveTreeModel::continue_white_main_line(const chessgame::Cursor &black_move, const NodePtr &parent_node, int move_number, bool is_main_line) -> void {
    if (auto white_continuation = black_move.child(0); white_continuation.has_value()) {
        build_subtree(parent_node, white_continuation.value(), move_number, is_main_line);
    }
}

auto MoveTreeModel::create_variations(const chessgame::Cursor &move, const NodePtr &parent_node, int move_number) -> void {
    for (size_t i = 1; i < move.child_count(); ++i) {
        if (auto variation = move.child(i); variation.has_value()) {
            build_subtree(parent_node, variation.value(), move_number, false);
        }
    }
}

auto MoveTreeModel::collect_black_continuation(const chessgame::Cursor &white_move, const NodePtr &current_node, int move_number, bool is_main_line) -> void {
    if (auto black_continuation = white_move.child(0); black_continuation.has_value()) {
        const auto &black_move = black_continuation.value();
        current_node->black_cursor = black_move;
        create_variations(black_move, current_node, move_number);
        continue_white_main_line(black_move, current_node->parent.lock(), move_number, is_main_line);
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
    const auto parent_is_white = parentModelNode->white_cursor && parentModelNode->white_cursor == parentCursor;
    if (parent_is_white) {
        handleMoveAddedToWhiteNode(parentModelNode, newCursor, childIndex);
    } else {
        handleMoveAddedToBlackNode(parentModelNode, newCursor, childIndex);
    }
}

auto MoveTreeModel::handleMoveAddedToWhiteNode(const NodePtr &modelNode, const chessgame::Cursor &newCursor, size_t childIndex) -> void {
    if (childIndex == 0) {
        modelNode->black_cursor = newCursor;
        auto idx = indexFromModelNode(modelNode);
        emit dataChanged(idx, index(idx.row(), ColumnCount - 1, idx.parent()));
    } else {
        auto newModelNode = std::make_shared<MoveTreeNode>();
        newModelNode->parent = modelNode;
        newModelNode->black_cursor = newCursor;
        newModelNode->move_number = modelNode->move_number;
        newModelNode->is_main_line = false;
        newModelNode->is_black_variation = true;

        auto parentIdx = indexFromModelNode(modelNode);
        int insertRow = static_cast<int>(modelNode->children.size());

        beginInsertRows(parentIdx, insertRow, insertRow);
        modelNode->children.push_back(newModelNode);
        endInsertRows();
    }
}

auto MoveTreeModel::handleMoveAddedToBlackNode(const NodePtr &modelNode, const chessgame::Cursor &newCursor, size_t childIndex) -> void {
    auto newModelNode = std::make_shared<MoveTreeNode>();
    newModelNode->white_cursor = newCursor;
    newModelNode->move_number = modelNode->move_number + 1;

    if (childIndex == 0) {
        auto parentNode = modelNode->parent.lock();
        if (!parentNode) {
            // This shouldn't happen, but handle gracefully
            rebuildTree();
            return;
        }

        newModelNode->parent = parentNode;
        newModelNode->is_main_line = modelNode->is_main_line;

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
        newModelNode->is_main_line = false;

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
    if ((node->white_cursor && node->white_cursor == cursor) || (node->black_cursor && node->black_cursor == cursor)) {
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
            return node->white_cursor ? moveText(*node->white_cursor) : QString();
        case BlackMoveColumn:
            return node->black_cursor ? moveText(*node->black_cursor) : QString();
        default:
            return {};
        }
    }

    if (index.column() == 0) {
        switch (role) {
            // case Qt::TextAlignmentRole:
            //     return Qt::AlignRight;

        case HasCommentRole:
            return (node->white_cursor && !node->white_cursor->comment().empty()) || (node->black_cursor && !node->black_cursor->comment().empty());

        case HasPremoveCommentRole:
            return (node->white_cursor && !node->white_cursor->premove_comment().empty()) || (node->black_cursor && !node->black_cursor->premove_comment().empty());

        case HasVariationsRole:
            return node->children.size() > 1;

        case IsMainLineRole:
            return node->is_main_line;

        case MoveNumberRole:
            return node->move_number;

        case IsWhiteVariationRole:
            return node->is_white_variation;

        case IsBlackVariationRole:
            return node->is_black_variation;

        case HasNagsRole:
            return (node->white_cursor && !node->white_cursor->nags().empty()) || (node->black_cursor && !node->black_cursor->nags().empty());

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

    if (index.column() == WhiteMoveColumn && node->white_cursor) {
        return node->white_cursor;
    }
    if (index.column() == BlackMoveColumn && node->black_cursor) {
        return node->black_cursor;
    }
    if (node->white_cursor) {
        return node->white_cursor;
    }
    return node->black_cursor;
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
    const auto cursor = (node->white_cursor) ? node->white_cursor : node->black_cursor;
    if (cursor && cursor->variation_number() > 0) {
        prefix = QString{"▶"};
    }

    QString move_number{};
    if (node->is_black_variation) {
        move_number = QString("%1...").arg(node->move_number);
    } else {
        move_number = QString("%1.").arg(node->move_number);
    }

    return prefix + move_number;
}

} // namespace chessgui
