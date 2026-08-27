/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#ifndef CHESS_GUI_PIECESET_H
#define CHESS_GUI_PIECESET_H

#include <QMap>
#include <QString>
#include <QSvgRenderer>

#include <array>
#include <memory>

#include <chess_core/piece.h>

namespace chess_gui {

class PieceSet {
public:
    explicit PieceSet(const QString &folder);
    PieceSet(const PieceSet &) = delete;
    PieceSet &operator=(const PieceSet &) = delete;

    auto renderer(chess_core::Piece piece) const -> const QSvgRenderer *;
private:
    std::array<std::unique_ptr<QSvgRenderer>, 2 * chess_core::piece_type_count> m_renderers{};
};

} // namespace chess_gui

#endif
