/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#ifndef CHESSGUI_CHESSBOARDWIDGET_H
#define CHESSGUI_CHESSBOARDWIDGET_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>

#include <array>

#include <chesscore/bitboard.h>
#include <chesscore/position.h>

#include "chessgui/GraphicsItems.h"
#include "chessgui/PieceSet.h"

namespace chessgui {

using Position = chesscore::Position<chesscore::Bitboard>;

class ChessboardWidget : public QGraphicsView {
    Q_OBJECT
public:
    explicit ChessboardWidget(const QString &piece_folder, QWidget *parent = nullptr);
    ChessboardWidget(const ChessboardWidget &) = delete;
    auto operator=(const ChessboardWidget &) -> ChessboardWidget & = delete;
    ChessboardWidget(ChessboardWidget &&) = default;
    auto operator=(ChessboardWidget &&) -> ChessboardWidget & = default;
    ~ChessboardWidget() override;

    auto showPosition(const Position &position) -> void;

    auto markSquare(const chesscore::Square &square) -> void;
    auto unmarkSquare(const chesscore::Square &square) -> void;
    auto clearMarkedSquares() -> void;

    auto pieceSet() const -> const PieceSet & { return m_piece_set; }
protected:
    auto resizeEvent(QResizeEvent *event) -> void override;
    auto mousePressEvent(QMouseEvent *event) -> void override;
signals:
    auto squareClicked(const chesscore::Square &square) -> void;
private:
    static constexpr auto brightSquareColor = QColor(252, 212, 146);
    static constexpr auto darkSquareColor = QColor(181, 155, 114);
    static constexpr auto squareHighlightColor = QColor(120, 255, 85, 100);

    auto drawBoard() -> void;
    auto placePieces(const Position &position) -> void;
    auto clearPieces() -> void;
    auto findSquareMarker(const chesscore::Square &square) -> std::optional<QGraphicsRectItem *>;

    static const qreal cell_size;
    QGraphicsScene *m_scene;
    PieceSet m_piece_set;
    std::array<ChessPiece *, 64> m_pieces{};
    QList<QPair<chesscore::Square, QGraphicsRectItem *>> m_markedSquares;
};

} // namespace chessgui

#endif
