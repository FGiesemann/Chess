/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#ifndef CHESSGUI_CHESSBOARDWIDGET_H
#define CHESSGUI_CHESSBOARDWIDGET_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>

#include <chesscore/bitboard.h>
#include <chesscore/position.h>

#include "chessgui/GraphicsItems.h"
#include "chessgui/PieceSet.h"

namespace chessgui {

using Position = chesscore::Position<chesscore::Bitboard>;

class ChessboardWidget : public QGraphicsView {
    Q_OBJECT
public:
    explicit ChessboardWidget(const PieceSet &pieces, QWidget *parent = nullptr);
    ChessboardWidget(const ChessboardWidget &) = delete;
    ChessboardWidget &operator=(const ChessboardWidget &) = delete;

    auto setPosition(const Position &position) -> void;

    auto markSquare(const chesscore::Square &square) -> void;
    auto unmarkSquare(const chesscore::Square &square) -> void;
    auto clearMarkedSquares() -> void;
protected:
    auto resizeEvent(QResizeEvent *event) -> void override;
    auto mousePressEvent(QMouseEvent *event) -> void override;
signals:
    auto squareClicked(const chesscore::Square &square) -> void;
private:
    auto drawBoard() -> void;
    auto placePieces(const Position &position) -> void;
    auto clearPieces() -> void;
    auto findSquareMarker(const chesscore::Square &square) -> std::optional<QGraphicsRectItem *>;

    QGraphicsScene *m_scene;
    const PieceSet &m_pieces;
    QMap<QPair<int, int>, ChessPiece *> m_piecemap;
    QList<QPair<chesscore::Square, QGraphicsRectItem *>> m_markedSquares;
};

} // namespace chessgui

#endif
