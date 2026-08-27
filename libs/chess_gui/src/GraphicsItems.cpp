/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#include "chess_gui/GraphicsItems.h"

namespace chess_gui {

ChessPiece::ChessPiece(const QSvgRenderer *renderer) {
    if (renderer != nullptr) {
        setSharedRenderer(const_cast<QSvgRenderer *>(renderer));
    } else {
        qWarning() << "Renderer for chess piece is nullptr";
    }
    setZValue(1);
}

} // namespace chess_gui
