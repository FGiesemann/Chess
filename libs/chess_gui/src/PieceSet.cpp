/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#include "chess_gui/PieceSet.h"
#include "chess_gui/ChessboardError.h"

#include <set>

#include <QDebug>
#include <QFile>

class PieceResourceInit {
public:
    PieceResourceInit() { Q_INIT_RESOURCE(pieces); }
    ~PieceResourceInit() { Q_CLEANUP_RESOURCE(pieces); }
};

static PieceResourceInit pieceResourceInit;

namespace chess_gui {

PieceSet::PieceSet(const QString &folder) {
    for (auto color : {chess_core::Color::White, chess_core::Color::Black}) {
        for (auto type : chess_core::all_piece_types) {
            const auto piece = chess_core::Piece{type, color};
            QString colorName = (color == chess_core::Color::White) ? "w" : "b";
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
            m_renderers[piece.dense_index()] = std::move(renderer);
        }
    }
}

auto PieceSet::renderer(chess_core::Piece piece) const -> const QSvgRenderer * {
    return m_renderers[piece.dense_index()].get();
}

} // namespace chess_gui
