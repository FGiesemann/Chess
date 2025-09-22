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

ChessboardWidget::ChessboardWidget(const PieceSet &pieces, QWidget *parent) : QGraphicsView(parent), m_pieces{pieces} {
    m_scene = new QGraphicsScene(this);
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setMouseTracking(true);

    m_scene->setSceneRect(0, 0, 8, 8);
}

auto ChessboardWidget::drawBoard() -> void {
    for (auto item : m_scene->items()) {
        if (qgraphicsitem_cast<QGraphicsRectItem *>(item)) {
            m_scene->removeItem(item);
            delete item;
        }
    }

    static constexpr auto brightSquareColor = QColor(252, 212, 146);
    static constexpr auto darkSquareColor = QColor(181, 155, 114);
    qreal cellSize = 1.0;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            auto *square = new QGraphicsRectItem(col, 7 - row, cellSize, cellSize);
            if ((row + col) % 2 == 1) {
                square->setBrush(QBrush(brightSquareColor));
                square->setPen(QPen(Qt::NoPen));
            } else {
                square->setBrush(QBrush(darkSquareColor));
                square->setPen(QPen(Qt::NoPen));
            }
            m_scene->addItem(square);
        }
    }
}

auto ChessboardWidget::setPosition(const Position &position) -> void {
    clearPieces();
    placePieces(position);
}

auto ChessboardWidget::clearPieces() -> void {
    for (const auto &item : m_piecemap) {
        m_scene->removeItem(item);
        delete item;
    }
    m_piecemap.clear();
}

auto ChessboardWidget::placePieces(const Position &position) -> void {
    const qreal cellSize = 1.0;

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            const auto piece_at_square = position.board().get_piece(chesscore::Square{col + 1, row + 1});
            if (piece_at_square.has_value()) {
                const auto *renderer = m_pieces.renderer(piece_at_square.value());
                auto *piece = new ChessPiece(renderer);
                QSizeF nativeSize = renderer->defaultSize();
                if (nativeSize.width() > 0 && nativeSize.height() > 0) {
                    qreal scaleX = cellSize / nativeSize.width();
                    qreal scaleY = cellSize / nativeSize.height();
                    piece->setTransform(QTransform::fromScale(scaleX, scaleY));
                    piece->setPos(col, 7 - row);
                    m_scene->addItem(piece);
                    m_piecemap.insert(QPair<int, int>(row, col), piece);
                } else {
                    qWarning() << "Warning: Skipping piece at" << row << "," << col << "due to invalid SVG graphics.";
                    delete piece;
                }
            }
        }
    }
}

auto ChessboardWidget::markSquare(const chesscore::Square &square) -> void {
    const auto opt_marker = findSquareMarker(square);
    if (opt_marker.has_value()) {
        return;
    }

    const qreal cellSize = 1.0;
    auto *marker = new QGraphicsRectItem(square.file().file - 1, 8 - square.rank().rank, cellSize, cellSize);
    QColor color{120, 255, 85};
    color.setAlpha(100);
    marker->setBrush(color);
    marker->setPen(Qt::NoPen);
    m_markedSquares.append({square, marker});
    m_scene->addItem(marker);
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
        m_scene->removeItem(opt_marker.value());
        delete opt_marker.value();
        m_markedSquares.removeOne({square, opt_marker.value()});
    }
}

auto ChessboardWidget::clearMarkedSquares() -> void {
    for (auto &item : m_markedSquares) {
        m_scene->removeItem(item.second);
        delete item.second;
    }
    m_markedSquares.clear();
}

auto ChessboardWidget::resizeEvent(QResizeEvent *event) -> void {
    QGraphicsView::resizeEvent(event);
    drawBoard();
    fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

auto ChessboardWidget::mousePressEvent(QMouseEvent *event) -> void {
    QGraphicsView::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        QPointF posInScene = mapToScene(event->pos());
        int file = qFloor(posInScene.x()) + 1;
        int rank = 8 - qFloor(posInScene.y());

        if (file > 0 && file <= 8 && rank > 0 && rank <= 8) {
            emit squareClicked(chesscore::Square{file, rank});
        }
    }
}

} // namespace chessgui
