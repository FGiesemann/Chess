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

ChessboardWidget::ChessboardWidget(const QString &piece_folder, QWidget *parent) : QGraphicsView(parent), m_scene{new QGraphicsScene(this)}, m_piece_set{piece_folder} {
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setMouseTracking(true);

    m_scene->setSceneRect(0, 0, chesscore::File::max_file, chesscore::Rank::max_rank);
    std::ranges::fill(m_pieces, nullptr);
}

auto ChessboardWidget::drawBoard() -> void {
    for (auto *item : m_scene->items()) {
        if (qgraphicsitem_cast<QGraphicsRectItem *>(item) != nullptr) {
            m_scene->removeItem(item);
            delete item;
        }
    }

    static constexpr auto brightSquareColor = QColor(252, 212, 146);
    static constexpr auto darkSquareColor = QColor(181, 155, 114);
    qreal cellSize = 1.0;

    for (int rank = chesscore::Rank::min_rank; rank <= chesscore::Rank::max_rank; ++rank) {
        for (int file = chesscore::File::min_file; file <= chesscore::File::max_file; ++file) {
            auto *square = new QGraphicsRectItem(file - 1, chesscore::Rank::max_rank - rank, cellSize, cellSize);
            if ((rank + file) % 2 == 1) {
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
    for (auto &piece : m_pieces) {
        if (piece != nullptr) {
            m_scene->removeItem(piece);
            delete piece;
            piece = nullptr;
        }
    }
}

auto ChessboardWidget::placePieces(const Position &position) -> void {
    const qreal cellSize = 1.0;

    for (int rank = chesscore::Rank::min_rank; rank <= chesscore::Rank::max_rank; ++rank) {
        for (int file = chesscore::File::min_file; file <= chesscore::File::max_file; ++file) {
            const auto square = chesscore::Square{file, rank};
            const auto piece_at_square = position.board().get_piece(square);
            if (piece_at_square.has_value()) {
                const auto *renderer = m_piece_set.renderer(piece_at_square.value());
                auto *piece = new ChessPiece(renderer);
                QSizeF nativeSize = renderer->defaultSize();
                if (nativeSize.width() > 0 && nativeSize.height() > 0) {
                    qreal scaleX = cellSize / nativeSize.width();
                    qreal scaleY = cellSize / nativeSize.height();
                    piece->setTransform(QTransform::fromScale(scaleX, scaleY));
                    piece->setPos(file - 1, chesscore::Rank::max_rank - rank);
                    m_scene->addItem(piece);
                    m_pieces[square.index()] = piece;
                } else {
                    qWarning() << "Warning: Skipping piece at" << rank << "," << file << "due to invalid SVG graphics.";
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
    auto *marker = new QGraphicsRectItem(square.file().file - 1, chesscore::Rank::max_rank - square.rank().rank, cellSize, cellSize);
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
        int rank = chesscore::Rank::max_rank - qFloor(posInScene.y());

        if (file >= chesscore::File::min_file && file <= chesscore::File::max_file && rank >= chesscore::Rank::min_rank && rank <= chesscore::Rank::max_rank) {
            emit squareClicked(chesscore::Square{file, rank});
        }
    }
}

} // namespace chessgui
