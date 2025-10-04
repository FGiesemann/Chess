/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#include "chessgui/MoveTreeModel.h"

#include <chessgame/san.h>

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

auto MoveTreeModel::onMoveAdded(const chessgame::NodeId &parentNodeId, size_t childIndex) -> void {
    if (!m_game) {
        return;
    }

    auto parentModelNode = findModelNodeByGameNodeId(parentNodeId);

    if (!parentModelNode) {
        rebuildTree();
        return;
    }

    auto cursor = m_game->const_cursor();
    GameNodePtr parentGameNode;

    std::function<GameNodePtr(GameNodePtr)> findGameNode;
    findGameNode = [&](const GameNodePtr &node) -> GameNodePtr {
        if (!node) {
            return nullptr;
        }
        if (node->id() == parentNodeId) {
            return node;
        }
        for (size_t i = 0; i < node->child_count(); ++i) {
            if (auto found = findGameNode(node->get_child(i))) {
                return found;
            }
        }
        return nullptr;
    };
    parentGameNode = findGameNode(cursor.node());

    if (!parentGameNode || childIndex >= parentGameNode->child_count()) {
        rebuildTree();
        return;
    }

    auto newGameNode = parentGameNode->get_child(childIndex);

    if (parentModelNode->whiteNode == parentGameNode) {
        handleMoveAddedToWhiteNode(parentModelNode, newGameNode, childIndex);
    } else if (parentModelNode->blackNode == parentGameNode) {
        handleMoveAddedToBlackNode(parentModelNode, newGameNode, childIndex);
    } else {
        // This shouldn't happen
        rebuildTree();
    }
}

auto MoveTreeModel::handleMoveAddedToWhiteNode(const std::shared_ptr<MoveTreeNode> &modelNode, const GameNodePtr &newGameNode, size_t childIndex) -> void {
    if (childIndex == 0) {
        modelNode->blackNode = newGameNode;
        auto idx = indexFromModelNode(modelNode);
        emit dataChanged(idx, index(idx.row(), ColumnCount - 1, idx.parent()));
    } else {
        auto newModelNode = std::make_shared<MoveTreeNode>();
        newModelNode->parent = modelNode;
        newModelNode->blackNode = newGameNode;
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

auto MoveTreeModel::handleMoveAddedToBlackNode(const std::shared_ptr<MoveTreeNode> &modelNode, const GameNodePtr &newGameNode, size_t childIndex) -> void {
    auto newModelNode = std::make_shared<MoveTreeNode>();
    newModelNode->whiteNode = newGameNode;
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
        parentNode->children.insert(parentNode->children.begin() + insertPos, newModelNode);
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

auto MoveTreeModel::onNodeDataChanged(const chessgame::NodeId &nodeId) -> void {
    auto modelNode = findModelNodeByGameNodeId(nodeId);

    if (!modelNode) {
        return;
    }

    auto idx = indexFromModelNode(modelNode);
    if (idx.isValid()) {
        emit dataChanged(idx, index(idx.row(), ColumnCount - 1, idx.parent()));
    }
}

auto MoveTreeModel::findModelNodeByGameNodeId(const chessgame::NodeId &nodeId) const -> std::shared_ptr<MoveTreeNode> {
    std::function<std::shared_ptr<MoveTreeNode>(const std::shared_ptr<MoveTreeNode> &)> search;
    search = [&](const std::shared_ptr<MoveTreeNode> &current) -> std::shared_ptr<MoveTreeNode> {
        if (!current) {
            return nullptr;
        }
        if ((current->whiteNode && current->whiteNode->id() == nodeId) || (current->blackNode && current->blackNode->id() == nodeId)) {
            return current;
        }
        for (const auto &child : current->children) {
            if (auto found = search(child)) {
                return found;
            }
        }
        return nullptr;
    };

    return search(m_root);
}

auto MoveTreeModel::findModelNodeByGameNode(const GameNodePtr &gameNode) const -> std::shared_ptr<MoveTreeNode> {
    if (!gameNode) {
        return nullptr;
    }
    return findModelNodeByGameNodeId(gameNode->id());
}

auto MoveTreeModel::indexFromModelNode(const std::shared_ptr<MoveTreeNode> &node, int column) const -> QModelIndex {
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

    auto cursor = m_game->const_cursor();

    int moveNumber = 1;

    if (cursor.child_count() > 0) {
        auto firstChild = cursor.child(0);
        if (firstChild) {
            buildSubtree(m_root, firstChild->node(), moveNumber, true);
        }
    }
}

auto MoveTreeModel::buildSubtree(const std::shared_ptr<MoveTreeNode> &parentModelNode, const GameNodePtr &gameNode, int moveNumber, bool isMainLine) -> void {
    if (!gameNode) {
        return;
    }
    auto pos = gameNode->calculate_position();
    bool isWhiteMove = (pos.side_to_move() == chesscore::Color::Black);

    std::shared_ptr<MoveTreeNode> currentModelNode;

    if (isWhiteMove) {
        currentModelNode = std::make_shared<MoveTreeNode>();
        currentModelNode->parent = parentModelNode;
        currentModelNode->whiteNode = gameNode;
        currentModelNode->moveNumber = moveNumber;
        currentModelNode->isMainLine = isMainLine;

        parentModelNode->children.push_back(currentModelNode);

        if (gameNode->child_count() > 0) {
            auto blackNode = gameNode->get_child(0);
            currentModelNode->blackNode = blackNode;

            if (blackNode->child_count() > 0) {
                buildSubtree(parentModelNode, blackNode->get_child(0), moveNumber + 1, isMainLine);
            }

            for (size_t i = 1; i < blackNode->child_count(); ++i) {
                buildSubtree(currentModelNode, blackNode->get_child(i), moveNumber + 1, false);
            }
        }

        for (size_t i = 1; i < gameNode->child_count(); ++i) {
            auto variationNode = std::make_shared<MoveTreeNode>();
            variationNode->parent = currentModelNode;
            variationNode->blackNode = gameNode->get_child(i);
            variationNode->moveNumber = moveNumber;
            variationNode->isMainLine = false;
            variationNode->isBlackVariation = true;

            currentModelNode->children.push_back(variationNode);

            auto varBlackNode = variationNode->blackNode;
            if (varBlackNode->child_count() > 0) {
                buildSubtree(variationNode, varBlackNode->get_child(0), moveNumber + 1, false);
            }

            for (size_t j = 1; j < varBlackNode->child_count(); ++j) {
                buildSubtree(variationNode, varBlackNode->get_child(j), moveNumber + 1, false);
            }
        }

    } else {
        currentModelNode = std::make_shared<MoveTreeNode>();
        currentModelNode->parent = parentModelNode;
        currentModelNode->blackNode = gameNode;
        currentModelNode->moveNumber = moveNumber;
        currentModelNode->isMainLine = false;
        currentModelNode->isBlackVariation = true;

        parentModelNode->children.push_back(currentModelNode);

        if (gameNode->child_count() > 0) {
            buildSubtree(parentModelNode, gameNode->get_child(0), moveNumber + 1, false);
        }

        for (size_t i = 1; i < gameNode->child_count(); ++i) {
            buildSubtree(currentModelNode, gameNode->get_child(i), moveNumber + 1, false);
        }
    }
}

auto MoveTreeModel::index(int row, int column, const QModelIndex &parent) const -> QModelIndex {
    if (!hasIndex(row, column, parent)) {
        return {};
    }

    auto parentNode = parent.isValid() ? modelNodeFromIndex(parent) : m_root;

    if (!parentNode || row >= static_cast<int>(parentNode->children.size())) {
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
            return node->whiteNode ? moveText(node->whiteNode) : QString();
        case BlackMoveColumn:
            return node->blackNode ? moveText(node->blackNode) : QString();
        default:
            return {};
        }
    }

    if (index.column() == 0) {
        switch (role) {
        case HasCommentRole:
            return (node->whiteNode && !node->whiteNode->comment().empty()) || (node->blackNode && !node->blackNode->comment().empty());

        case HasPremoveCommentRole:
            return (node->whiteNode && !node->whiteNode->premove_comment().empty()) || (node->blackNode && !node->blackNode->premove_comment().empty());

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
            return (node->whiteNode && !node->whiteNode->nags().empty()) || (node->blackNode && !node->blackNode->nags().empty());

        case NodeIdRole:
            if (node->whiteNode) {
                return QVariant::fromValue(node->whiteNode->id());
            } else if (node->blackNode) {
                return QVariant::fromValue(node->blackNode->id());
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

auto MoveTreeModel::nodeFromIndex(const QModelIndex &index) const -> GameNodePtr {
    if (!index.isValid()) {
        return nullptr;
    }

    auto node = modelNodeFromIndex(index);
    if (!node) {
        return nullptr;
    }

    if (index.column() == WhiteMoveColumn && node->whiteNode) {
        return node->whiteNode;
    }
    if (index.column() == BlackMoveColumn && node->blackNode) {
        return node->blackNode;
    }
    if (node->whiteNode) {
        return node->whiteNode;
    }
    return node->blackNode;
}

auto MoveTreeModel::modelNodeFromIndex(const QModelIndex &index) const -> std::shared_ptr<MoveTreeNode> {
    if (!index.isValid()) {
        return nullptr;
    }

    auto *ptr = static_cast<MoveTreeNode *>(index.internalPointer());
    if (ptr == nullptr) {
        return nullptr;
    }

    std::function<std::shared_ptr<MoveTreeNode>(const std::shared_ptr<MoveTreeNode> &, MoveTreeNode *)> findNode;
    findNode = [&](const std::shared_ptr<MoveTreeNode> &current, MoveTreeNode *target) -> std::shared_ptr<MoveTreeNode> {
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

auto MoveTreeModel::moveText(const GameNodePtr &node) -> QString {
    if (!node) {
        return {};
    }
    const auto parent = node->parent();
    if (parent == nullptr) {
        return QString{"No Parent"};
    }
    const auto position = node->calculate_position();
    const auto check_state = position.check_state();
    QString check_marker = (check_state == chesscore::CheckState::Check) ? QString{"+"} : (check_state == chesscore::CheckState::Checkmate) ? QString{"#"} : QString{""};
    const auto parent_position = parent->calculate_position();
    const auto all_moves = parent_position.all_legal_moves();
    const auto &move = node->move();
    const auto opt_san = chessgame::generate_san_move(move, all_moves);
    if (opt_san.has_value()) {
        return QString::fromStdString(opt_san.value().san_string) + check_marker;
    }
    return QString{"No SAN move"};
}

auto MoveTreeModel::moveNumberText(const std::shared_ptr<MoveTreeNode> &node, int column) -> QString {
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
