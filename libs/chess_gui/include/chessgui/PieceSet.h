/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#ifndef CHESSGUI_PIECESET_H
#define CHESSGUI_PIECESET_H

#include <QMap>
#include <QString>
#include <QSvgRenderer>

#include <map>
#include <memory>

#include <chesscore/piece.h>

namespace chessgui {

class PieceSet {
public:
    explicit PieceSet(const QString &folder);
    PieceSet(const PieceSet &) = delete;
    PieceSet &operator=(const PieceSet &) = delete;

    auto renderer(chesscore::Piece piece) const -> const QSvgRenderer *;

    using PieceKey = std::pair<chesscore::PieceType, chesscore::Color>;
private:
    std::map<PieceKey, std::unique_ptr<QSvgRenderer>> m_renderers;
};

} // namespace chessgui

#endif
