/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#ifndef CHESS_GUI_CHESSBOARDWIDGET_H
#define CHESS_GUI_CHESSBOARDWIDGET_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>

#include <array>

#include <chess_core/bitboard.h>
#include <chess_core/position.h>

#include "chess_gui/GraphicsItems.h"
#include "chess_gui/PieceSet.h"

namespace chess_gui {

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

    auto showPosition(const chess_core::Position &position) -> void;

    auto markSquare(const chess_core::Square &square) -> void;
    auto unmarkSquare(const chess_core::Square &square) -> void;
    auto clearMarkedSquares() -> void;

    auto pieceSet() const -> const PieceSet & { return m_piece_set; }

    auto setGhostPiece(chess_core::Piece piece, chess_core::Square square) -> void;
    auto clearGhostPiece() -> void;
    auto hidePiece(chess_core::Square square) -> void;
    auto showPiece(chess_core::Square square) -> void;

    auto showPromotionSelection(chess_core::Color color, chess_core::Square target_square) -> void;
    auto cancelPromotionPieceSelection() -> void;
protected:
    auto resizeEvent(QResizeEvent *event) -> void override;
    auto mousePressEvent(QMouseEvent *event) -> void override;
    auto mouseMoveEvent(QMouseEvent *event) -> void override;
    auto mouseReleaseEvent(QMouseEvent *event) -> void override;
    auto keyPressEvent(QKeyEvent *event) -> void override;
signals:
    auto mousePressed(const chess_core::Square &square) -> void;
    auto mouseReleased(const chess_core::Square &square) -> void;
    auto cancelRequested() -> void;
    auto promotionPieceSelected(chess_core::PieceType type) -> void;
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
    auto placePieces(const chess_core::Position &position) -> void;
    auto create_piece_item(chess_core::Piece piece, qreal piece_size, QPointF pos) -> ChessPiece *;
    auto clearPieces() -> void;
    auto findSquareMarker(const chess_core::Square &square) -> std::optional<QGraphicsRectItem *>;
    auto squareAt(const QPoint &pos) -> std::optional<chess_core::Square>;
    auto cleanupPromotionOverlay() -> void;
    static auto create_promotion_piece_selection_rect(qreal item_x, qreal selection_item_size, chess_core::PieceType &piece_type) -> QGraphicsRectItem *;
    static auto calculate_overlay_pos(chess_core::Square target_square, QSizeF overlay_size) -> QPointF;

    static const qreal cell_size;
    QGraphicsScene m_scene;
    PieceSet m_piece_set;
    std::array<ChessPiece *, chess_core::File::count * chess_core::Rank::count> m_pieces{};
    QList<QPair<chess_core::Square, QGraphicsRectItem *>> m_markedSquares;
    State m_state{State::Normal};
    ChessPiece *m_ghost_piece{};
    QGraphicsItemGroup *m_promotion_overlay_group{};
};

} // namespace chess_gui

#endif
