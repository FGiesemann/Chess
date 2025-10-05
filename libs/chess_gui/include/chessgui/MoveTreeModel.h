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

    auto cursorFromIndex(const QModelIndex &index) const -> std::optional<chessgame::Cursor>;
public slots:
    auto onMoveAdded(const chessgame::Cursor &parentCursor, size_t childIndex) -> void;
    auto onNodeDataChanged(const chessgame::Cursor &cursor) -> void;
    auto rebuildTree() -> void;
signals:
    void gameChanged();
private:
    struct MoveTreeNode;
    using NodePtr = std::shared_ptr<MoveTreeNode>;

    struct MoveTreeNode {
        std::weak_ptr<MoveTreeNode> parent;
        std::vector<NodePtr> children;

        std::optional<chessgame::Cursor> whiteCursor; ///< The white half-move
        std::optional<chessgame::Cursor> blackCursor; ///< The black half-move (may be nullptr)

        int moveNumber{1};            ///< Full move number
        bool isMainLine{true};        ///< True if this is part of the main line
        bool isWhiteVariation{false}; ///< True if this starts a white variation
        bool isBlackVariation{false}; ///< True if this starts a black variation
    };

    auto buildTree() -> void;
    auto buildSubtree(const NodePtr &parentModelNode, const chessgame::Cursor &cursor, int moveNumber, bool isMainLine) -> void;
    static auto make_model_node(const NodePtr &parent, const chessgame::Cursor &cursor, int moveNumber, bool isMainline) -> NodePtr;

    auto modelNodeFromIndex(const QModelIndex &index) const -> NodePtr;
    static auto searchForCursor(const chessgame::Cursor &cursor, const NodePtr &node) -> NodePtr;
    auto modelNodeByCursor(const chessgame::Cursor &cursor) const -> NodePtr;
    auto indexFromModelNode(const NodePtr &node, int column = 0) const -> QModelIndex;

    static auto moveText(const chessgame::Cursor &cursor) -> QString;
    static auto moveNumberText(const NodePtr &node, int column) -> QString;

    auto handleMoveAddedToWhiteNode(const NodePtr &modelNode, const chessgame::Cursor &newCursor, size_t childIndex) -> void;
    auto handleMoveAddedToBlackNode(const NodePtr &modelNode, const chessgame::Cursor &newCursor, size_t childIndex) -> void;

    GamePtr m_game;
    NodePtr m_root;
};

} // namespace chessgui

#endif
