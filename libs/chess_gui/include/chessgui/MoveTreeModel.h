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

/**
 * @brief Model for displaying a chess game tree in a QTreeView.
 *
 * This model presents chess moves in a tree structure with three columns:
 * - Column 0: Move number (with markers for variations)
 * - Column 1: White's move
 * - Column 2: Black's move
 *
 * Each row represents a full move (white + black), with variations shown as child items.
 *
 * The model observes changes to the game tree and updates incrementally when notified.
 */
class MoveTreeModel : public QAbstractItemModel {
    Q_OBJECT
public:
    // Custom roles for additional data
    enum CustomRoles {
        HasCommentRole = Qt::UserRole + 1, ///< bool: Node has a comment
        HasPremoveCommentRole,             ///< bool: Node has a premove comment
        HasVariationsRole,                 ///< bool: Node has variations (multiple children)
        IsMainLineRole,                    ///< bool: This move is on the main line
        NodeIdRole,                        ///< NodeId: ID of the GameNode
        MoveNumberRole,                    ///< int: The move number
        IsWhiteVariationRole,              ///< bool: This is a variation starting with white
        IsBlackVariationRole,              ///< bool: This is a variation starting with black
        HasNagsRole                        ///< bool: Node has NAGs (Numeric Annotation Glyphs)
    };
    Q_ENUM(CustomRoles)

    enum Column { MoveNumberColumn = 0, WhiteMoveColumn = 1, BlackMoveColumn = 2, ColumnCount = 3 };

    explicit MoveTreeModel(QObject *parent = nullptr);
    explicit MoveTreeModel(std::shared_ptr<chessgame::Game> game, QObject *parent = nullptr);
    ~MoveTreeModel() override = default;

    // Set the game to display
    auto setGame(std::shared_ptr<chessgame::Game> game) -> void;
    auto game() const -> std::shared_ptr<chessgame::Game> { return m_game; }

    // QAbstractItemModel interface
    auto index(int row, int column, const QModelIndex &parent = QModelIndex()) const -> QModelIndex override;
    auto parent(const QModelIndex &child) const -> QModelIndex override;
    auto rowCount(const QModelIndex &parent = QModelIndex()) const -> int override;
    auto columnCount(const QModelIndex &parent = QModelIndex()) const -> int override;
    auto data(const QModelIndex &index, int role = Qt::DisplayRole) const -> QVariant override;
    auto headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const -> QVariant override;
    auto flags(const QModelIndex &index) const -> Qt::ItemFlags override;

    std::shared_ptr<const chessgame::GameNode> nodeFromIndex(const QModelIndex &index) const;
public slots:
    auto onMoveAdded(const chessgame::NodeId &parentNodeId, size_t childIndex) -> void;
    auto onNodeDataChanged(const chessgame::NodeId &nodeId) -> void;
    auto rebuildTree() -> void;
signals:
    void gameChanged();
private:
    struct ModelNode;

    struct ModelNode {
        std::weak_ptr<ModelNode> parent;
        std::vector<std::shared_ptr<ModelNode>> children;

        std::shared_ptr<chessgame::GameNode> whiteNode; ///< The white half-move
        std::shared_ptr<chessgame::GameNode> blackNode; ///< The black half-move (may be nullptr)

        int moveNumber{1};            ///< Full move number
        bool isMainLine{true};        ///< True if this is part of the main line
        bool isWhiteVariation{false}; ///< True if this starts a white variation
        bool isBlackVariation{false}; ///< True if this starts a black variation
    };

    auto buildTree() -> void;
    auto buildSubtree(std::shared_ptr<ModelNode> parentModelNode, std::shared_ptr<chessgame::GameNode> gameNode, int moveNumber, bool isMainLine) -> void;

    auto modelNodeFromIndex(const QModelIndex &index) const -> std::shared_ptr<ModelNode>;
    auto findModelNodeByGameNodeId(const chessgame::NodeId &nodeId) const -> std::shared_ptr<ModelNode>;
    auto findModelNodeByGameNode(std::shared_ptr<chessgame::GameNode> gameNode) const -> std::shared_ptr<ModelNode>;
    auto indexFromModelNode(const std::shared_ptr<ModelNode> &node, int column = 0) const -> QModelIndex;

    auto moveText(const std::shared_ptr<chessgame::GameNode> &node) const -> QString;
    auto moveNumberText(const std::shared_ptr<ModelNode> &node, int column) const -> QString;

    auto handleMoveAddedToWhiteNode(std::shared_ptr<ModelNode> modelNode, std::shared_ptr<chessgame::GameNode> newGameNode, size_t childIndex) -> void;
    auto handleMoveAddedToBlackNode(std::shared_ptr<ModelNode> modelNode, std::shared_ptr<chessgame::GameNode> newGameNode, size_t childIndex) -> void;

    std::shared_ptr<chessgame::Game> m_game;
    std::shared_ptr<ModelNode> m_root; ///< Virtual root node
};

} // namespace chessgui

#endif
