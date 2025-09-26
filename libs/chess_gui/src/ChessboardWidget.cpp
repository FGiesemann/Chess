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

    m_scene.setSceneRect(0, 0, chesscore::File::max_file, chesscore::Rank::max_rank);
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

    for (int rank = chesscore::Rank::min_rank; rank <= chesscore::Rank::max_rank; ++rank) {
        for (int file = chesscore::File::min_file; file <= chesscore::File::max_file; ++file) {
            auto *square = new QGraphicsRectItem(file - 1, chesscore::Rank::max_rank - rank, cell_size, cell_size);
            if ((rank + file) % 2 == 1) {
                square->setBrush(QBrush(brightSquareColor));
                square->setPen(QPen(Qt::NoPen));
            } else {
                square->setBrush(QBrush(darkSquareColor));
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

auto ChessboardWidget::placePieces(const chesscore::Position &position) -> void {
    for (int rank = chesscore::Rank::min_rank; rank <= chesscore::Rank::max_rank; ++rank) {
        for (int file = chesscore::File::min_file; file <= chesscore::File::max_file; ++file) {
            const auto square = chesscore::Square{file, rank};
            const auto piece_at_square = position.board().get_piece(square);
            if (piece_at_square.has_value()) {
                const auto *renderer = m_piece_set.renderer(piece_at_square.value());
                auto *piece = new ChessPiece(renderer);
                QSizeF nativeSize = renderer->defaultSize();
                qreal scaleX = cell_size / nativeSize.width();
                qreal scaleY = cell_size / nativeSize.height();
                piece->setTransform(QTransform::fromScale(scaleX, scaleY));
                piece->setPos(file - 1, chesscore::Rank::max_rank - rank);
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

    auto *marker = new QGraphicsRectItem(square.file().file - 1, chesscore::Rank::max_rank - square.rank().rank, cell_size, cell_size);
    marker->setBrush(squareHighlightColor);
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

    const QSvgRenderer *renderer = m_piece_set.renderer(piece);
    if (renderer != nullptr) {
        m_ghost_piece = new ChessPiece(renderer);
        QSizeF nativeSize = renderer->defaultSize();
        qreal scaleX = cell_size / nativeSize.width();
        qreal scaleY = cell_size / nativeSize.height();
        m_ghost_piece->setTransform(QTransform::fromScale(scaleX, scaleY));
        m_ghost_piece->setPos(square.file().file - 1, chesscore::Rank::max_rank - square.rank().rank);
        m_scene.addItem(m_ghost_piece);
    }
}

auto ChessboardWidget::clearGhostPiece() -> void {
    if (m_ghost_piece != nullptr) {
        m_scene.removeItem(m_ghost_piece);
        delete m_ghost_piece;
        m_ghost_piece = nullptr;
    }
}

auto ChessboardWidget::showPromotionSelection(chesscore::Color color, chesscore::Square target_square) -> void {
    if (m_promotionOverlayGroup != nullptr) {
        cleanupPromotionOverlay();
    }
    clearGhostPiece();

    m_state = State::SelectingPromotionPiece;

    qreal pieceSize = cell_size * promotion_piece_scale;
    qreal selectionItemSize = pieceSize + 2 * cell_size * promotion_piece_padding;
    qreal overlayWidth = selectionItemSize * 4;
    qreal overlayHeight = selectionItemSize;

    QPointF centerPos = QPointF(target_square.file().file - 1, chesscore::Rank::max_rank - target_square.rank().rank) + QPointF{.5, .5};
    qreal idealX = centerPos.x() - overlayWidth / 2.0;
    qreal idealY = centerPos.y() - overlayHeight / 2.0;

    qreal boardSize = cell_size * chesscore::File::max_file;

    qreal clampedX = std::clamp(idealX, 0.0, boardSize - overlayWidth);
    qreal clampedY = std::clamp(idealY, 0.0, boardSize - overlayHeight);

    m_promotionOverlayGroup = new QGraphicsItemGroup();
    m_scene.addItem(m_promotionOverlayGroup);

    auto *background = new QGraphicsRectItem(0, 0, overlayWidth, overlayHeight);
    background->setBrush(QBrush(QColor(100, 100, 100)));
    background->setPen(QPen(Qt::black, .01F));
    m_promotionOverlayGroup->addToGroup(background);

    m_promotionOverlayGroup->setPos(clampedX, clampedY);

    for (size_t i = 0; i < chesscore::all_promotion_piece_types.size(); ++i) {
        chesscore::PieceType pieceType = chesscore::all_promotion_piece_types[i];
        qreal itemX = i * selectionItemSize;
        auto *selectionRect = new QGraphicsRectItem(clampedX + itemX, clampedY, selectionItemSize, selectionItemSize);
        selectionRect->setBrush(QBrush(QColor(150, 150, 150)));
        selectionRect->setPen(Qt::NoPen);
        m_promotionOverlayGroup->addToGroup(selectionRect);

        selectionRect->setData(0, QVariant::fromValue(get_index(pieceType)));
        auto *renderer = m_piece_set.renderer(chesscore::Piece{.type = pieceType, .color = color});
        auto *pieceItem = new ChessPiece(renderer);

        QSizeF nativeSize = renderer->defaultSize();
        qreal scaleX = pieceSize / nativeSize.width();
        qreal scaleY = pieceSize / nativeSize.height();
        pieceItem->setTransform(QTransform::fromScale(scaleX, scaleY));

        qreal pieceOffsetX = clampedX + itemX + promotion_piece_padding;
        qreal pieceOffsetY = clampedY + promotion_piece_padding;
        pieceItem->setPos(pieceOffsetX, pieceOffsetY);

        m_promotionOverlayGroup->addToGroup(pieceItem);
    }

    m_promotionOverlayGroup->setZValue(100);

    viewport()->update();
}

auto ChessboardWidget::cleanupPromotionOverlay() -> void {
    if (m_promotionOverlayGroup) {
        m_scene.removeItem(m_promotionOverlayGroup);
        delete m_promotionOverlayGroup;
        m_promotionOverlayGroup = nullptr;
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
    QPointF posInScene = mapToScene(pos);
    int file = qFloor(posInScene.x()) + 1;
    int rank = chesscore::Rank::max_rank - qFloor(posInScene.y());

    if (file >= chesscore::File::min_file && file <= chesscore::File::max_file && rank >= chesscore::Rank::min_rank && rank <= chesscore::Rank::max_rank) {
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
                if (item->group() == m_promotionOverlayGroup) {
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
