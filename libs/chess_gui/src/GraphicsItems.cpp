/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#include "chessgui/GraphicsItems.h"

namespace chessgui {

ChessPiece::ChessPiece(const QSvgRenderer *renderer) {
    if (renderer) {
        setSharedRenderer(const_cast<QSvgRenderer *>(renderer));
    } else {
        qWarning() << "Renderer for chess piece is nullptr";
    }
    setZValue(1);
}

} // namespace chessgui
