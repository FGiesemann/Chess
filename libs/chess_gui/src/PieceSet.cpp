/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#include "chessgui/PieceSet.h"

#include <set>

#include <QDebug>
#include <QFile>

namespace chessgui {

PieceSet::PieceSet(const QString &folder) {
    for (auto color : {chesscore::Color::White, chesscore::Color::Black}) {
        for (auto type : chesscore::all_piece_types) {
            const auto piece = chesscore::Piece{.type = type, .color = color};
            QString colorName = (color == chesscore::Color::White) ? "w" : "b";
            QString fileName = QString("%1/%2%3.svg").arg(folder, colorName, QString{piece.piece_char_colorless()}.toLower());
            if (!QFile::exists(fileName)) {
                qWarning() << "Error: Piece file " << fileName << " does not exist!"; // TODO: exception
            }
            auto renderer = std::make_unique<QSvgRenderer>(fileName);
            if (!renderer->isValid()) {
                qWarning() << QString("Fatal error: Invalid SVG file for chess piece at path: %1").arg(fileName);
                continue;
            }

            m_renderers[piece.piece_index()] = std::move(renderer);
        }
    }
}

auto PieceSet::renderer(chesscore::Piece piece) const -> const QSvgRenderer * {
    return m_renderers[piece.piece_index()].get();
}

} // namespace chessgui
