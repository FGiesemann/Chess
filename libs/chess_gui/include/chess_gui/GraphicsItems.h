/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#ifndef CHESS_GUI_GRAPHICS_ITEMS_H
#define CHESS_GUI_GRAPHICS_ITEMS_H

#include <QGraphicsSvgItem>
#include <QSvgRenderer>

#include <chess_core/piece.h>

namespace chess_gui {

class ChessPiece : public QGraphicsSvgItem {
public:
    explicit ChessPiece(const QSvgRenderer *renderer);
};

} // namespace chess_gui

#endif
