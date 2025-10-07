/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#ifndef CHESSGUI_MOVETREEMODEL_H
#define CHESSGUI_MOVETREEMODEL_H

#include <QAbstractItemModel>
#include <memory>

#include <chessgame/game.h>

namespace chessgui {

class MoveTreeModel : public QAbstractItemModel {
    Q_OBJECT

    using GamePtr = std::shared_ptr<chessgame::Game>;
public:
    enum CustomRoles {
        HasCommentRole = Qt::UserRole + 1, ///< bool: Node has a comment
        HasPremoveCommentRole,             ///< bool: Node has a premove comment
        HasVariationsRole,                 ///< bool: Node has variations (multiple children)
        IsMainLineRole,                    ///< bool: This move is on the main line
        MoveNumberRole,                    ///< int: The move number
        IsWhiteVariationRole,              ///< bool: This is a variation starting with white
        IsBlackVariationRole,              ///< bool: This is a variation starting with black
        HasNagsRole                        ///< bool: Node has NAGs (Numeric Annotation Glyphs)
    };
    Q_ENUM(CustomRoles)

    enum Column { MoveNumberColumn = 0, WhiteMoveColumn = 1, BlackMoveColumn = 2, ColumnCount = 3 };

    explicit MoveTreeModel(QObject *parent = nullptr);
    explicit MoveTreeModel(GamePtr game, QObject *parent = nullptr);

    auto setGame(GamePtr game) -> void;
    auto game() const -> GamePtr { return m_game; }

    auto index(int row, int column, const QModelIndex &parent = QModelIndex()) const -> QModelIndex override;
    auto parent(const QModelIndex &child) const -> QModelIndex override;
    auto rowCount(const QModelIndex &parent = QModelIndex()) const -> int override;
    auto columnCount(const QModelIndex &parent = QModelIndex()) const -> int override;
    auto data(const QModelIndex &index, int role = Qt::DisplayRole) const -> QVariant override;
    auto headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const -> QVariant override;
    auto flags(const QModelIndex &index) const -> Qt::ItemFlags override;

    auto cursor_from_index(const QModelIndex &index) const -> std::optional<chessgame::Cursor>;
public slots:
    auto onMoveAdded(const chessgame::Cursor &parent_cursor, size_t child_index) -> void;
    auto onNodeDataChanged(const chessgame::Cursor &cursor) -> void;
    auto rebuild_tree() -> void;
signals:
    void gameChanged();
private:
    struct MoveTreeNode;
    using NodePtr = std::shared_ptr<MoveTreeNode>;

    struct MoveTreeNode {
        std::weak_ptr<MoveTreeNode> parent;
        std::vector<NodePtr> children;

        std::optional<chessgame::Cursor> white_cursor; ///< The white half-move
        std::optional<chessgame::Cursor> black_cursor; ///< The black half-move (may be nullptr)

        int move_number{1};             ///< Full move number
        bool is_main_line{true};        ///< True if this is part of the main line
        bool is_white_variation{false}; ///< True if this starts a white variation
        bool is_black_variation{false}; ///< True if this starts a black variation
    };

    auto build_tree() -> void;
    static auto build_subtree(const NodePtr &parent_node, const chessgame::Cursor &move, int move_number, bool is_main_line) -> void;
    static auto continue_white_main_line(const chessgame::Cursor &black_move, const NodePtr &parent_node, int move_number, bool is_main_line) -> void;
    static auto create_variations(const chessgame::Cursor &move, const NodePtr &parent_node, int move_number) -> void;
    static auto collect_black_continuation(const chessgame::Cursor &white_move, const NodePtr &current_node, int move_number, bool is_main_line) -> void;
    static auto make_model_node(const NodePtr &parent, const chessgame::Cursor &cursor, int move_number, bool is_main_line, bool is_black_variation = false) -> NodePtr;

    auto model_node_from_index(const QModelIndex &index) const -> NodePtr;
    static auto search_for_cursor(const chessgame::Cursor &cursor, const NodePtr &node) -> NodePtr;
    auto model_node_by_cursor(const chessgame::Cursor &cursor) const -> NodePtr;
    auto index_from_model_node(const NodePtr &node, int column = 0) const -> QModelIndex;

    static auto move_text(const chessgame::Cursor &cursor) -> QString;
    static auto move_number_text(const NodePtr &node, int column) -> QString;

    auto handle_black_move_added(const NodePtr &model_node, const chessgame::Cursor &black_move, size_t child_index) -> void;
    auto handle_white_move_added(const NodePtr &model_node, const chessgame::Cursor &white_move, size_t child_index) -> void;

    GamePtr m_game;
    NodePtr m_root;
};

} // namespace chessgui

#endif
