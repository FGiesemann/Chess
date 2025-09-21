/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#include "chessgui/PieceSet.h"

#include <QDebug>
#include <QFile>

namespace chessgui {

PieceSet::PieceSet(const QString &folder) {
    QMap<chesscore::PieceType, QString> pieceNames;
    pieceNames[chesscore::PieceType::Pawn] = "p";
    pieceNames[chesscore::PieceType::Rook] = "r";
    pieceNames[chesscore::PieceType::Knight] = "n";
    pieceNames[chesscore::PieceType::Bishop] = "b";
    pieceNames[chesscore::PieceType::Queen] = "q";
    pieceNames[chesscore::PieceType::King] = "k";

    for (chesscore::Color color : {chesscore::Color::White, chesscore::Color::Black}) {
        for (chesscore::PieceType type : pieceNames.keys()) {
            QString colorName = (color == chesscore::Color::White) ? "w" : "b";
            QString fileName = QString("%1/%2%3.svg").arg(folder, colorName, pieceNames.value(type));
            if (!QFile::exists(fileName)) {
                qWarning() << "Error: Piece file " << fileName << " does not exist!"; // TODO: exception
            }
            auto renderer = std::make_unique<QSvgRenderer>(fileName);
            if (!renderer->isValid()) {
                qWarning() << QString("Fatal error: Invalid SVG file for chess piece at path: %1").arg(fileName);
                continue;
            }

            m_renderers.insert({PieceKey{type, color}, std::move(renderer)});
        }
    }
}

auto PieceSet::renderer(chesscore::Piece piece) const -> const QSvgRenderer * {
    auto it = m_renderers.find({piece.type, piece.color});
    if (it != m_renderers.end()) {
        return it->second.get();
    }
    return nullptr;
}

} // namespace chessgui
