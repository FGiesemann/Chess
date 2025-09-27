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

class ChessboardWidget : public QGraphicsView {
    Q_OBJECT
public:
    enum class State { Normal, SelectingPromotionPiece };

    explicit ChessboardWidget(QWidget *parent = nullptr);
    explicit ChessboardWidget(const QString &piece_folder, QWidget *parent = nullptr);
    ChessboardWidget(const ChessboardWidget &) = delete;
    auto operator=(const ChessboardWidget &) -> ChessboardWidget & = delete;
    ChessboardWidget(ChessboardWidget &&) = default;
    auto operator=(ChessboardWidget &&) -> ChessboardWidget & = default;
    ~ChessboardWidget() = default;

    auto showPosition(const chesscore::Position &position) -> void;

    auto markSquare(const chesscore::Square &square) -> void;
    auto unmarkSquare(const chesscore::Square &square) -> void;
    auto clearMarkedSquares() -> void;

    auto pieceSet() const -> const PieceSet & { return m_piece_set; }

    auto setGhostPiece(chesscore::Piece piece, chesscore::Square square) -> void;
    auto clearGhostPiece() -> void;
    auto hidePiece(chesscore::Square square) -> void;
    auto showPiece(chesscore::Square square) -> void;

    auto showPromotionSelection(chesscore::Color color, chesscore::Square target_square) -> void;
    auto cancelPromotionPieceSelection() -> void;
protected:
    auto resizeEvent(QResizeEvent *event) -> void override;
    auto mousePressEvent(QMouseEvent *event) -> void override;
    auto mouseMoveEvent(QMouseEvent *event) -> void override;
    auto mouseReleaseEvent(QMouseEvent *event) -> void override;
    auto keyPressEvent(QKeyEvent *event) -> void override;
signals:
    auto mousePressed(const chesscore::Square &square) -> void;
    auto mouseReleased(const chesscore::Square &square) -> void;
    auto cancelRequested() -> void;
    auto promotionPieceSelected(chesscore::PieceType type) -> void;
private:
    static constexpr qreal half = 0.5F;
    static constexpr qreal minimal_boundary = 0.01F;
    static constexpr int promotion_piece_selection_z_value = 5;
    static constexpr auto bright_square_color = QColor(252, 212, 146);
    static constexpr auto dark_square_color = QColor(181, 155, 114);
    static constexpr auto target_highlight_color = QColor(120, 255, 85, 100);
    static constexpr auto promotion_piece_selection_background_color = QColor(100, 100, 100);
    static constexpr auto promotion_piece_selection_rect_color = QColor(150, 150, 150, 150);

    static constexpr qreal promotion_piece_scale{0.75F};
    static constexpr qreal promotion_piece_padding{0.05F};

    auto drawBoard() -> void;
    auto placePieces(const chesscore::Position &position) -> void;
    auto clearPieces() -> void;
    auto findSquareMarker(const chesscore::Square &square) -> std::optional<QGraphicsRectItem *>;
    auto squareAt(const QPoint &pos) -> std::optional<chesscore::Square>;
    auto cleanupPromotionOverlay() -> void;

    static const qreal cell_size;
    QGraphicsScene m_scene;
    PieceSet m_piece_set;
    std::array<ChessPiece *, chesscore::File::max_file * chesscore::Rank::max_rank> m_pieces{};
    QList<QPair<chesscore::Square, QGraphicsRectItem *>> m_markedSquares;
    State m_state{State::Normal};
    ChessPiece *m_ghost_piece{};
    QGraphicsItemGroup *m_promotionOverlayGroup{};
};

} // namespace chessgui

#endif
