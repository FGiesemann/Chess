/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#include "chessgui/ChessboardWidget.h"
#include "chessgui/ChessPieceItem.h"

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
                auto *piece = new ChessPiece(piece_at_square.value(), renderer);
                QSizeF nativeSize = renderer->defaultSize();
                if (nativeSize.width() > 0 && nativeSize.height() > 0) {
                    qreal scaleX = cellSize / nativeSize.width();
                    qreal scaleY = cellSize / nativeSize.height();
                    qWarning() << QString("Piece size: (%1, %2)").arg(nativeSize.width()).arg(nativeSize.height());
                    qWarning() << QString("Cell size: (%1, %2)").arg(cellSize).arg(cellSize);
                    qWarning() << QString("Scaling factors: (%1, %2)").arg(scaleX).arg(scaleY);
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

void ChessboardWidget::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    drawBoard();
    fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

} // namespace chessgui
