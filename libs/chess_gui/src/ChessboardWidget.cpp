/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#include "chessgui/ChessboardWidget.h"

#include <QBrush>
#include <QGraphicsRectItem>
#include <QResizeEvent>
#include <QtGlobal>

namespace chessgui {

const qreal ChessboardWidget::cell_size{1.0};

ChessboardWidget::ChessboardWidget(const QString &piece_folder, QWidget *parent) : QGraphicsView(parent), m_scene{this}, m_piece_set{piece_folder} {
    setScene(&m_scene);
    setRenderHint(QPainter::Antialiasing);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setMouseTracking(true);

    m_scene.setSceneRect(0, 0, chesscore::File::count, chesscore::Rank::count);
    std::ranges::fill(m_pieces, nullptr);
}

ChessboardWidget::ChessboardWidget(QWidget *parent) : ChessboardWidget(":/pieces", parent) {}

auto ChessboardWidget::drawBoard() -> void {
    for (auto *item : m_scene.items()) {
        if (qgraphicsitem_cast<QGraphicsRectItem *>(item) != nullptr) {
            m_scene.removeItem(item);
            delete item;
        }
    }

    for (int rank = 0; rank < chesscore::Rank::count; ++rank) {
        for (int file = 0; file < chesscore::File::count; ++file) {
            auto *square = new QGraphicsRectItem(file - 1, chesscore::Rank::count - rank, cell_size, cell_size);
            if ((rank + file) % 2 == 1) {
                square->setBrush(QBrush(bright_square_color));
                square->setPen(QPen(Qt::NoPen));
            } else {
                square->setBrush(QBrush(dark_square_color));
                square->setPen(QPen(Qt::NoPen));
            }
            m_scene.addItem(square);
        }
    }
}

auto ChessboardWidget::showPosition(const chesscore::Position &position) -> void {
    clearPieces();
    placePieces(position);
}

auto ChessboardWidget::clearPieces() -> void {
    for (auto &piece : m_pieces) {
        if (piece != nullptr) {
            m_scene.removeItem(piece);
            delete piece;
            piece = nullptr;
        }
    }
}

auto ChessboardWidget::create_piece_item(chesscore::Piece piece, qreal piece_size, QPointF pos) -> ChessPiece * {
    const auto *renderer = m_piece_set.renderer(piece);
    auto *piece_item = new ChessPiece(renderer);
    QSizeF native_size = renderer->defaultSize();
    qreal scaleX = piece_size / native_size.width();
    qreal scaleY = piece_size / native_size.height();
    piece_item->setTransform(QTransform::fromScale(scaleX, scaleY));
    piece_item->setPos(pos);
    return piece_item;
}

auto ChessboardWidget::placePieces(const chesscore::Position &position) -> void {
    for (int rank = 0; rank < chesscore::Rank::count; ++rank) {
        for (int file = 0; file < chesscore::File::count; ++file) {
            const auto square = chesscore::Square{file, rank};
            const auto piece_at_square = position.board().get_piece(square);
            if (piece_at_square.has_value()) {
                QPointF piece_pos = QPointF(file - 1, chesscore::Rank::count - rank);
                auto *piece = create_piece_item(piece_at_square.value(), cell_size, piece_pos);
                m_scene.addItem(piece);
                m_pieces[square.index()] = piece;
            }
        }
    }
}

auto ChessboardWidget::markSquare(const chesscore::Square &square) -> void {
    const auto opt_marker = findSquareMarker(square);
    if (opt_marker.has_value()) {
        return;
    }

    auto *marker = new QGraphicsRectItem(square.file().file - 1, chesscore::Rank::count - 1 - square.rank().rank, cell_size, cell_size);
    marker->setBrush(target_highlight_color);
    marker->setPen(Qt::NoPen);
    m_markedSquares.append({square, marker});
    m_scene.addItem(marker);
}

auto ChessboardWidget::findSquareMarker(const chesscore::Square &square) -> std::optional<QGraphicsRectItem *> {
    for (const auto &marker : m_markedSquares) {
        if (marker.first == square) {
            return marker.second;
        }
    }
    return {};
}

auto ChessboardWidget::unmarkSquare(const chesscore::Square &square) -> void {
    auto opt_marker = findSquareMarker(square);
    if (opt_marker.has_value()) {
        m_scene.removeItem(opt_marker.value());
        delete opt_marker.value();
        m_markedSquares.removeOne({square, opt_marker.value()});
    }
}

auto ChessboardWidget::clearMarkedSquares() -> void {
    for (auto &item : m_markedSquares) {
        m_scene.removeItem(item.second);
        delete item.second;
    }
    m_markedSquares.clear();
}

auto ChessboardWidget::setGhostPiece(chesscore::Piece piece, chesscore::Square square) -> void {
    clearGhostPiece();

    QPointF piece_pos = QPointF(square.file().file - 1, chesscore::Rank::count - 1 - square.rank().rank);
    m_ghost_piece = create_piece_item(piece, cell_size, piece_pos);
    m_scene.addItem(m_ghost_piece);
}

auto ChessboardWidget::clearGhostPiece() -> void {
    if (m_ghost_piece != nullptr) {
        m_scene.removeItem(m_ghost_piece);
        delete m_ghost_piece;
        m_ghost_piece = nullptr;
    }
}

auto ChessboardWidget::showPromotionSelection(chesscore::Color color, chesscore::Square target_square) -> void {
    if (m_promotion_overlay_group != nullptr) {
        cleanupPromotionOverlay();
    }
    clearGhostPiece();

    m_state = State::SelectingPromotionPiece;

    qreal piece_size = cell_size * promotion_piece_scale;
    qreal selection_item_size = piece_size + 2 * cell_size * promotion_piece_padding;
    QSizeF overlay_size = QSizeF{selection_item_size * 4, selection_item_size};

    m_promotion_overlay_group = new QGraphicsItemGroup();
    m_scene.addItem(m_promotion_overlay_group);

    auto *background = new QGraphicsRectItem(0, 0, overlay_size.width(), overlay_size.height());
    background->setBrush(QBrush(promotion_piece_selection_background_color));
    background->setPen(QPen(Qt::black, minimal_boundary));
    m_promotion_overlay_group->addToGroup(background);

    for (size_t piece_index = 0; piece_index < chesscore::all_promotion_piece_types.size(); ++piece_index) {
        chesscore::PieceType piece_type = chesscore::all_promotion_piece_types[piece_index];
        qreal item_x = static_cast<qreal>(piece_index) * selection_item_size;
        m_promotion_overlay_group->addToGroup(create_promotion_piece_selection_rect(item_x, selection_item_size, piece_type));
        QPointF piece_pos = QPointF(item_x + promotion_piece_padding, 0);
        m_promotion_overlay_group->addToGroup(create_piece_item(chesscore::Piece{.type = piece_type, .color = color}, piece_size, piece_pos));
    }

    m_promotion_overlay_group->setPos(calculate_overlay_pos(target_square, overlay_size));
    m_promotion_overlay_group->setZValue(promotion_piece_selection_z_value);

    viewport()->update();
}

auto ChessboardWidget::calculate_overlay_pos(chesscore::Square target_square, QSizeF overlay_size) -> QPointF {
    QPointF center_pos = QPointF(target_square.file().file - 1, chesscore::Rank::count - 1 - target_square.rank().rank) + QPointF{half, half};
    QPointF ideal_pos = center_pos - QPointF{overlay_size.width(), overlay_size.height()} * half;
    qreal board_width = cell_size * chesscore::File::count;
    return QPointF{std::clamp(ideal_pos.x(), 0.0, board_width - overlay_size.width()), std::clamp(ideal_pos.y(), 0.0, board_width - overlay_size.height())};
}

auto ChessboardWidget::create_promotion_piece_selection_rect(qreal item_x, qreal selection_item_size, chesscore::PieceType &piece_type) -> QGraphicsRectItem * {
    auto *selection_rect = new QGraphicsRectItem(item_x, 0, selection_item_size, selection_item_size);
    selection_rect->setBrush(QBrush(promotion_piece_selection_rect_color));
    selection_rect->setPen(Qt::NoPen);
    selection_rect->setData(0, QVariant::fromValue(get_index(piece_type)));
    return selection_rect;
}

auto ChessboardWidget::cancelPromotionPieceSelection() -> void {
    cleanupPromotionOverlay();
}

auto ChessboardWidget::cleanupPromotionOverlay() -> void {
    if (m_promotion_overlay_group != nullptr) {
        m_scene.removeItem(m_promotion_overlay_group);
        delete m_promotion_overlay_group;
        m_promotion_overlay_group = nullptr;
    }
    m_state = State::Normal;
    viewport()->update();
}

auto ChessboardWidget::hidePiece(chesscore::Square square) -> void {
    if (m_pieces[square.index()] != nullptr) {
        m_pieces[square.index()]->setVisible(false);
    }
}

auto ChessboardWidget::showPiece(chesscore::Square square) -> void {
    if (m_pieces[square.index()] != nullptr) {
        m_pieces[square.index()]->setVisible(true);
    }
}

auto ChessboardWidget::squareAt(const QPoint &pos) -> std::optional<chesscore::Square> {
    QPointF pos_in_scene = mapToScene(pos);
    int file = qFloor(pos_in_scene.x()) + 1;
    int rank = chesscore::Rank::count - qFloor(pos_in_scene.y());

    if (file >= 0 && file < chesscore::File::count && rank >= 0 && rank < chesscore::Rank::count) {
        return chesscore::Square{file, rank};
    }
    return {};
}

auto ChessboardWidget::resizeEvent(QResizeEvent *event) -> void {
    QGraphicsView::resizeEvent(event);
    drawBoard();
    fitInView(m_scene.sceneRect(), Qt::KeepAspectRatio);
}

auto ChessboardWidget::mousePressEvent(QMouseEvent *event) -> void {
    QGraphicsView::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        if (m_state == State::Normal) {
            const auto opt_square = squareAt(event->pos());
            if (opt_square.has_value()) {
                emit mousePressed(opt_square.value());
            }
        } else if (m_state == State::SelectingPromotionPiece) {
            QPointF posInScene = mapToScene(event->pos());
            QList<QGraphicsItem *> clickedItems = m_scene.items(posInScene);
            chesscore::PieceType type = chesscore::PieceType::Pawn;
            for (QGraphicsItem *item : clickedItems) {
                if (item->group() == m_promotion_overlay_group) {
                    QVariant item_data = item->data(0);
                    if (item_data.isValid()) {
                        type = chesscore::piece_type_from_index(item_data.toInt());
                        break;
                    }
                }
            }

            if (type != chesscore::PieceType::Pawn) {
                emit promotionPieceSelected(type);
                cleanupPromotionOverlay();
            }
        }
    }
}

auto ChessboardWidget::mouseMoveEvent(QMouseEvent *event) -> void {
    static const qreal shift = cell_size / 2.0;
    if (m_ghost_piece != nullptr) {
        QPointF posInScene = mapToScene(event->pos());
        m_ghost_piece->setPos(posInScene - QPointF(shift, shift));
    }
    QGraphicsView::mouseMoveEvent(event);
}

auto ChessboardWidget::mouseReleaseEvent(QMouseEvent *event) -> void {
    QGraphicsView::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton && m_state == State::Normal) {
        const auto opt_square = squareAt(event->pos());
        if (opt_square.has_value()) {
            emit mouseReleased(opt_square.value());
        }
    }
}

auto ChessboardWidget::keyPressEvent(QKeyEvent *event) -> void {
    if (event->key() == Qt::Key_Escape) {
        emit cancelRequested();
    }
}

} // namespace chessgui
