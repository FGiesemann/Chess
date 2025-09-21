/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#ifndef CHESSGUI_CHESSPIECEITEM_H
#define CHESSGUI_CHESSPIECEITEM_H

#include <QGraphicsSvgItem>
#include <QSvgRenderer>

#include <chesscore/piece.h>

namespace chessgui {

class ChessPiece : public QGraphicsSvgItem {
public:
    explicit ChessPiece(chesscore::Piece piece, const QSvgRenderer *renderer);
private:
    chesscore::Piece m_piece;
};

} // namespace chessgui

#endif
