/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#ifndef CHESS_GUI_CHESSBOARDCONTROLLER_H
#define CHESS_GUI_CHESSBOARDCONTROLLER_H

#include <QObject>

#include "chess_gui/ChessboardWidget.h"

namespace chess_gui {

class ChessboardController : public QObject {
    Q_OBJECT
public:
    ChessboardController(ChessboardWidget *board_widget, QObject *parent = nullptr);

    auto mark_target_squares(bool mark) -> void { m_mark_target_squares = mark; }
    auto make_move(const chess_core::Move &move) -> void;
    auto enable_user_interaction() -> void { m_user_interaction_enabled = true; };
    auto disable_user_interaction() -> void { m_user_interaction_enabled = false; };
    auto is_user_interaction_enabled() -> bool { return m_user_interaction_enabled; };

    auto current_position() const -> const chess_core::Position & { return m_current_position; }
    auto current_position() -> chess_core::Position & { return m_current_position; }
    auto set_position(const chess_core::Position &position) -> void;
signals:
    auto piece_selected(const chess_core::Square &square, chess_core::Piece piece) -> void;
    auto move_cancelled() -> void;
    auto move_made(const chess_core::Move &move) -> void;
private slots:
    auto on_square_clicked(const chess_core::Square &square) -> void;
    auto on_square_released(const chess_core::Square &square) -> void;
    auto on_cancel_requested() -> void;
    auto on_promotion_piece_selected(chess_core::PieceType type) -> void;
private:
    ChessboardWidget *m_board_widget;
    chess_core::Position m_current_position;
    bool m_mark_target_squares{false};
    chess_core::Move m_promotion_move{};
    bool m_user_interaction_enabled{true};

    std::optional<chess_core::Square> m_selected_square{};
    chess_core::MoveList m_legal_moves{};

    auto start_possible_move(chess_core::Square square, chess_core::Piece piece) -> void;
    auto try_move(chess_core::Square square) -> void;
    auto perform_move(const chess_core::Move &move) -> void;
    auto cancel_move() -> void;
    auto compute_piece_moves(chess_core::Square square) -> void;
};

} // namespace chess_gui

#endif
