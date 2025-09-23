/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#ifndef CHESSGUI_PIECESET_H
#define CHESSGUI_PIECESET_H

#include <QMap>
#include <QString>
#include <QSvgRenderer>

#include <array>
#include <memory>

#include <chesscore/piece.h>

namespace chessgui {

class PieceSet {
public:
    explicit PieceSet(const QString &folder);
    PieceSet(const PieceSet &) = delete;
    PieceSet &operator=(const PieceSet &) = delete;

    auto renderer(chesscore::Piece piece) const -> const QSvgRenderer *;
private:
    std::array<std::unique_ptr<QSvgRenderer>, 12> m_renderers{};
};

} // namespace chessgui

#endif
