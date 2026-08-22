/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#ifndef CHESSGUI_GRAPHICS_ITEMS_H
#define CHESSGUI_GRAPHICS_ITEMS_H

#include <QGraphicsSvgItem>
#include <QSvgRenderer>

#include <chesscore/piece.h>

namespace chessgui {

class ChessPiece : public QGraphicsSvgItem {
public:
    explicit ChessPiece(const QSvgRenderer *renderer);
};

} // namespace chessgui

#endif
