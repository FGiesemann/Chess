/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#include "chessgui/PieceSet.h"
#include "chessgui/ChessboardError.h"

#include <set>

#include <QDebug>
#include <QFile>

namespace chessgui {

class PieceResourceInit {
public:
    PieceResourceInit() { Q_INIT_RESOURCE(pieces); }
    ~PieceResourceInit() { Q_CLEANUP_RESOURCE(pieces); }
};

static PieceResourceInit pieceResourceInit;

PieceSet::PieceSet(const QString &folder) {
    for (auto color : {chesscore::Color::White, chesscore::Color::Black}) {
        for (auto type : chesscore::all_piece_types) {
            const auto piece = chesscore::Piece{.type = type, .color = color};
            QString colorName = (color == chesscore::Color::White) ? "w" : "b";
            QString fileName = QString("%1/%2%3.svg").arg(folder, colorName, QString{piece.piece_char_colorless()}.toLower());
            if (!QFile::exists(fileName)) {
                throw ChessboardError{QString{"Piece file %1 does not exist"}.arg(fileName)};
            }
            auto renderer = std::make_unique<QSvgRenderer>(fileName);
            if (!renderer->isValid()) {
                throw ChessboardError{QString{"Invalid SVG file for chess piece at path "} + fileName};
            }
            QSizeF nativeSize = renderer->defaultSize();
            if (nativeSize.width() <= 0 || nativeSize.height() <= 0) {
                throw ChessboardError{QString{"Invalid SVG file for chess piece at path "} + fileName};
            }
            m_renderers[piece.piece_index()] = std::move(renderer);
        }
    }
}

auto PieceSet::renderer(chesscore::Piece piece) const -> const QSvgRenderer * {
    return m_renderers[piece.piece_index()].get();
}

} // namespace chessgui
