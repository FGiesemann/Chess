/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#include "chessgui/ChessPieceItem.h"

namespace chessgui {

ChessPiece::ChessPiece(chesscore::Piece piece, const QSvgRenderer *renderer) : m_piece{piece} {
    if (renderer) {
        setSharedRenderer(const_cast<QSvgRenderer *>(renderer));
    } else {
        qWarning() << "Renderer for chess piece is nullptr";
    }
    setZValue(1);
}

} // namespace chessgui
