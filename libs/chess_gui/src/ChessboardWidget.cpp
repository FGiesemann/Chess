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

ChessboardWidget::ChessboardWidget(QWidget *parent) : QGraphicsView(parent) {
    m_scene = new QGraphicsScene(this);
    setScene(m_scene);

    setRenderHint(QPainter::Antialiasing);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setMouseTracking(true);
}

void ChessboardWidget::drawBoard() {
    qreal cellSize = qMin(viewport()->width(), viewport()->height()) / 8.0;
    qreal boardSize = cellSize * 8;

    m_scene->setSceneRect(0, 0, boardSize, boardSize);

    m_scene->clear();

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            QGraphicsRectItem *square = new QGraphicsRectItem(col * cellSize, row * cellSize, cellSize, cellSize);
            if ((row + col) % 2 == 0) {
                square->setBrush(QBrush(Qt::white));
            } else {
                square->setBrush(QBrush(Qt::black));
            }
            m_scene->addItem(square);
        }
    }

    centerOn(boardSize / 2, boardSize / 2);
}

void ChessboardWidget::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    drawBoard();
}

} // namespace chessgui
