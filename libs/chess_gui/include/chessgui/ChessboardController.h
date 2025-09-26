/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#ifndef CHESSGUI_CHESSBOARDCONTROLLER_H
#define CHESSGUI_CHESSBOARDCONTROLLER_H

#include <QObject>

#include "chessgui/ChessboardWidget.h"

namespace chessgui {

class ChessboardController : public QObject {
    Q_OBJECT
public:
    ChessboardController(ChessboardWidget *board_widget, QObject *parent = nullptr);

    auto mark_target_squares(bool mark) -> void { m_mark_target_squares = mark; }
signals:
    auto piece_selected(const chesscore::Square &square, chesscore::Piece piece) -> void;
    auto move_cancelled() -> void;
    auto move_made(const chesscore::Move &move) -> void;
private slots:
    auto on_square_clicked(const chesscore::Square &square) -> void;
    auto on_square_released(const chesscore::Square &square) -> void;
    auto on_cancel_requested() -> void;
    auto on_promotion_piece_selected(chesscore::PieceType type) -> void;
private:
    ChessboardWidget *m_board_widget;
    chesscore::Position m_current_position;
    bool m_mark_target_squares{false};
    chesscore::Move m_promotion_move{};

    std::optional<chesscore::Square> m_selected_square{};
    chesscore::MoveList m_legal_moves{};

    auto start_possible_move(chesscore::Square square, chesscore::Piece piece) -> void;
    auto try_move(chesscore::Square square) -> void;
    auto perform_move(const chesscore::Move &move) -> void;
    auto cancel_move() -> void;
    auto compute_piece_moves(chesscore::Square square) -> void;
};

} // namespace chessgui

#endif
