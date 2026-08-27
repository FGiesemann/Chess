/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#include "chess_gui/MoveTreeModel.h"

#include <chess_game/san.h>

#include <utility>

namespace chess_gui {

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

auto MoveTreeModel::rebuild_tree() -> void {
    beginResetModel();
    build_tree();
    endResetModel();
}

auto MoveTreeModel::build_tree() -> void {
    m_root = std::make_shared<MoveTreeNode>();

    if (!m_game) {
        return;
    }

    const int move_number = 1;
    for (size_t i = 0; i < m_game->cursor().child_count(); ++i) {
        if (const auto child = m_game->cursor().child(i); child.has_value()) {
            build_subtree(m_root, child.value(), move_number, i == 0);
        }
    }
}

auto MoveTreeModel::build_subtree(const NodePtr &parent_node, const chess_game::Cursor &move, int move_number, bool is_main_line) -> void {
    if (const auto is_white_move = move.player_color() == chess_core::Color::White; is_white_move) {
        auto move_node = make_model_node(parent_node, move, move_number, is_main_line);
        parent_node->children.push_back(move_node);
        create_variations(move, move_node, move_number);
        collect_black_continuation(move, move_node, move_number + 1, is_main_line);
    } else {
        auto move_node = make_model_node(parent_node, move, move_number, false, true);
        parent_node->children.push_back(move_node);
        create_variations(move, move_node, move_number + 1);
        continue_white_main_line(move, parent_node, move_number + 1, false);
    }
}

auto MoveTreeModel::make_model_node(const NodePtr &parent, const chess_game::Cursor &cursor, int move_number, bool is_main_line, bool is_black_variation) -> NodePtr {
    auto model_node = std::make_shared<MoveTreeNode>();
    model_node->parent = parent;
    if (cursor.player_color() == chess_core::Color::White) {
        model_node->white_cursor = cursor;
    } else {
        model_node->black_cursor = cursor;
    }
    model_node->move_number = move_number;
    model_node->is_main_line = is_main_line;
    model_node->is_black_variation = is_black_variation;
    return model_node;
}

auto MoveTreeModel::continue_white_main_line(const chess_game::Cursor &black_move, const NodePtr &parent_node, int move_number, bool is_main_line) -> void {
    if (auto white_continuation = black_move.child(0); white_continuation.has_value()) {
        build_subtree(parent_node, white_continuation.value(), move_number, is_main_line);
    }
}

auto MoveTreeModel::create_variations(const chess_game::Cursor &move, const NodePtr &parent_node, int move_number) -> void {
    for (size_t i = 1; i < move.child_count(); ++i) {
        if (auto variation = move.child(i); variation.has_value()) {
            build_subtree(parent_node, variation.value(), move_number, false);
        }
    }
}

auto MoveTreeModel::collect_black_continuation(const chess_game::Cursor &white_move, const NodePtr &current_node, int move_number, bool is_main_line) -> void {
    if (auto black_continuation = white_move.child(0); black_continuation.has_value()) {
        const auto &black_move = black_continuation.value();
        current_node->black_cursor = black_move;
        create_variations(black_move, current_node, move_number);
        continue_white_main_line(black_move, current_node->parent.lock(), move_number, is_main_line);
    }
}

auto MoveTreeModel::onMoveAdded(const chess_game::Cursor &parent_cursor, size_t child_index) -> void {
    if (m_game == nullptr) {
        return;
    }

    auto parent_model_node = model_node_by_cursor(parent_cursor);
    if (parent_model_node == nullptr) {
        rebuild_tree();
        return;
    }

    auto child_cursor_opt = parent_cursor.child(child_index);
    if (!child_cursor_opt.has_value()) {
        rebuild_tree();
        return;
    }
    const auto &new_cursor = *child_cursor_opt;
    const auto parent_is_white = parent_model_node->white_cursor && parent_model_node->white_cursor == parent_cursor;
    if (parent_is_white) {
        handle_black_move_added(parent_model_node, new_cursor, child_index);
    } else {
        handle_white_move_added(parent_model_node, new_cursor, child_index);
    }
}

auto MoveTreeModel::handle_black_move_added(const NodePtr &model_node, const chess_game::Cursor &black_move, size_t child_index) -> void {
    if (child_index == 0) {
        model_node->black_cursor = black_move;
        auto idx = index_from_model_node(model_node);
        emit dataChanged(idx, index(idx.row(), BlackMoveColumn, idx.parent()));
    } else {
        auto new_model_node = make_model_node(model_node, black_move, model_node->move_number, false, true);
        auto parent_idx = index_from_model_node(model_node);
        int insert_row = static_cast<int>(model_node->children.size());
        beginInsertRows(parent_idx, insert_row, insert_row);
        model_node->children.push_back(new_model_node);
        endInsertRows();
    }
}

auto MoveTreeModel::handle_white_move_added(const NodePtr &model_node, const chess_game::Cursor &white_move, size_t child_index) -> void {
    auto parent_node = model_node->parent.lock();
    if (!parent_node) {
        // This shouldn't happen, but handle gracefully
        rebuild_tree();
        return;
    }
    auto new_model_node =
        make_model_node(child_index == 0 ? parent_node : model_node, white_move, model_node->move_number + 1, child_index == 0 ? model_node->is_main_line : false, false);

    if (child_index == 0) {
        auto parent_idx = index_from_model_node(parent_node);
        size_t insert_pos = 0;
        for (size_t i = 0; i < parent_node->children.size(); ++i) {
            if (parent_node->children[i] == model_node) {
                insert_pos = i + 1;
                break;
            }
        }

        beginInsertRows(parent_idx, static_cast<int>(insert_pos), static_cast<int>(insert_pos));
        parent_node->children.insert(parent_node->children.begin() + static_cast<int>(insert_pos), new_model_node);
        endInsertRows();
    } else {
        auto parent_idx = index_from_model_node(model_node);
        int insert_row = static_cast<int>(model_node->children.size());

        beginInsertRows(parent_idx, insert_row, insert_row);
        model_node->children.push_back(new_model_node);
        endInsertRows();
    }
}

auto MoveTreeModel::onNodeDataChanged(const chess_game::Cursor &cursor) -> void {
    auto model_node = model_node_by_cursor(cursor);

    if (!model_node) {
        return;
    }

    auto idx = index_from_model_node(model_node);
    if (idx.isValid()) {
        emit dataChanged(idx, index(idx.row(), ColumnCount - 1, idx.parent()));
    }
}

auto MoveTreeModel::search_for_cursor(const chess_game::Cursor &cursor, const NodePtr &node) -> NodePtr {
    if (node == nullptr) {
        return nullptr;
    }
    if ((node->white_cursor && node->white_cursor == cursor) || (node->black_cursor && node->black_cursor == cursor)) {
        return node;
    }
    for (const auto &child : node->children) {
        if (auto found = search_for_cursor(cursor, child)) {
            return found;
        }
    }
    return nullptr;
}

auto MoveTreeModel::model_node_by_cursor(const chess_game::Cursor &cursor) const -> NodePtr {
    if (!cursor.node()) {
        return nullptr;
    }
    return search_for_cursor(cursor, m_root);
}

auto MoveTreeModel::index_from_model_node(const NodePtr &node, int column) const -> QModelIndex {
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

    auto parent_node = parent.isValid() ? model_node_from_index(parent) : m_root;

    if (!parent_node || std::cmp_greater_equal(row, parent_node->children.size())) {
        return {};
    }

    auto child_node = parent_node->children[row];
    return createIndex(row, column, child_node.get());
}

auto MoveTreeModel::parent(const QModelIndex &child) const -> QModelIndex {
    if (!child.isValid()) {
        return {};
    }

    auto child_node = model_node_from_index(child);
    if (!child_node) {
        return {};
    }

    auto parent_node = child_node->parent.lock();
    if (!parent_node || parent_node == m_root) {
        return {};
    }

    auto grand_parent_node = parent_node->parent.lock();
    if (!grand_parent_node) {
        return {};
    }

    for (size_t i = 0; i < grand_parent_node->children.size(); ++i) {
        if (grand_parent_node->children[i] == parent_node) {
            return createIndex(static_cast<int>(i), 0, parent_node.get());
        }
    }

    return {};
}

auto MoveTreeModel::rowCount(const QModelIndex &parent) const -> int {
    if (parent.column() > 0) {
        return 0;
    }

    auto parent_node = parent.isValid() ? model_node_from_index(parent) : m_root;
    return parent_node ? static_cast<int>(parent_node->children.size()) : 0;
}

auto MoveTreeModel::columnCount(const QModelIndex &parent) const -> int {
    Q_UNUSED(parent)
    return ColumnCount;
}

auto MoveTreeModel::data(const QModelIndex &index, int role) const -> QVariant {
    if (!index.isValid()) {
        return {};
    }

    auto node = model_node_from_index(index);
    if (!node) {
        return {};
    }

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case MoveNumberColumn:
            return move_number_text(node, index.column());
        case WhiteMoveColumn:
            return node->white_cursor ? move_text(*node->white_cursor) : QString();
        case BlackMoveColumn:
            return node->black_cursor ? move_text(*node->black_cursor) : QString();
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

auto MoveTreeModel::cursor_from_index(const QModelIndex &index) const -> std::optional<chess_game::Cursor> {
    if (!index.isValid()) {
        return std::nullopt;
    }

    auto node = model_node_from_index(index);
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

auto MoveTreeModel::model_node_from_index(const QModelIndex &index) const -> NodePtr {
    if (!index.isValid()) {
        return nullptr;
    }

    auto *ptr = static_cast<MoveTreeNode *>(index.internalPointer());
    if (ptr == nullptr) {
        return nullptr;
    }

    std::function<NodePtr(const NodePtr &, MoveTreeNode *)> find_node;
    find_node = [&](const NodePtr &current, MoveTreeNode *target) -> NodePtr {
        if (current.get() == target) {
            return current;
        }
        for (const auto &child : current->children) {
            if (auto found = find_node(child, target)) {
                return found;
            }
        }
        return nullptr;
    };

    return find_node(m_root, ptr);
}

auto MoveTreeModel::move_text(const chess_game::Cursor &cursor) -> QString {
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
    const auto opt_san = chess_game::generate_san_move(move, all_moves);
    if (opt_san.has_value()) {
        return QString::fromStdString(opt_san.value().san_string) + check_marker;
    }
    return QString{"No SAN move"};
}

auto MoveTreeModel::move_number_text(const NodePtr &node, int column) -> QString {
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

} // namespace chess_gui
