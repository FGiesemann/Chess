/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#include "chessgui/ChessboardController.h"

namespace chessgui {

static const chesscore::FenString max_promotions{"8/PPPPPPPP/8/8/8/8/pppppppp/8 w - - 0 1"};

ChessboardController::ChessboardController(ChessboardWidget *board_widget, QObject *parent) : QObject(parent), m_board_widget{board_widget}, m_current_position{max_promotions} {

    connect(m_board_widget, &ChessboardWidget::mousePressed, this, &ChessboardController::on_square_clicked);
    connect(m_board_widget, &ChessboardWidget::mouseReleased, this, &ChessboardController::on_square_released);
    connect(m_board_widget, &ChessboardWidget::cancelRequested, this, &ChessboardController::on_cancel_requested);
    connect(m_board_widget, &ChessboardWidget::promotionPieceSelected, this, &ChessboardController::on_promotion_piece_selected);
    m_board_widget->showPosition(m_current_position);
}

auto ChessboardController::on_square_clicked(const chesscore::Square &square) -> void {
    if (!m_user_interaction_enabled) {
        return;
    }

    if (!m_selected_square.has_value()) {
        const auto opt_piece = m_current_position.board().get_piece(square);
        if (opt_piece.has_value() && opt_piece.value().color == m_current_position.side_to_move()) {
            start_possible_move(square, opt_piece.value());
        }
    } else {
        if (square == m_selected_square.value()) {
            cancel_move();
        } else {
            try_move(square);
        }
    }
}

auto ChessboardController::on_square_released(const chesscore::Square &square) -> void {
    if (!m_user_interaction_enabled) {
        return;
    }

    if (m_selected_square.has_value() && square != m_selected_square.value()) {
        try_move(square);
    }
}

auto ChessboardController::on_cancel_requested() -> void {
    if (!m_user_interaction_enabled) {
        return;
    }

    cancel_move();
}

auto ChessboardController::on_promotion_piece_selected(chesscore::PieceType type) -> void {
    if (!m_user_interaction_enabled) {
        return;
    }

    if (m_promotion_move.promoted.has_value()) {
        m_promotion_move.promoted.value().type = type;
    }
    perform_move(m_promotion_move);
}

auto ChessboardController::start_possible_move(chesscore::Square square, chesscore::Piece piece) -> void {
    emit piece_selected(square, piece);
    m_selected_square = square;
    m_board_widget->clearMarkedSquares();
    m_board_widget->setGhostPiece(piece, square);
    m_board_widget->hidePiece(square);
    compute_piece_moves(square);
}

auto ChessboardController::try_move(chesscore::Square square) -> void {
    const auto iter = std::ranges::find_if(m_legal_moves, [&](const chesscore::Move &move) { return move.to == square; });
    if (iter != m_legal_moves.end()) {
        const auto move = *iter;
        if (move.is_pawn_promotion()) {
            m_promotion_move = move;
            m_board_widget->showPromotionSelection(move.piece.color, move.to);
        } else {
            perform_move(*iter);
        }
    } else {
        cancel_move();
    }
}

auto ChessboardController::perform_move(const chesscore::Move &move) -> void {
    emit move_made(move);
    m_current_position.make_move(move);
    m_board_widget->showPosition(m_current_position);
    m_board_widget->clearMarkedSquares();
    m_board_widget->clearGhostPiece();
    m_selected_square = std::nullopt;
    m_legal_moves.clear();
}

auto ChessboardController::cancel_move() -> void {
    emit move_cancelled();
    m_board_widget->clearMarkedSquares();
    m_board_widget->clearGhostPiece();
    m_board_widget->cancelPromotionPieceSelection();
    if (m_selected_square.has_value()) {
        m_board_widget->showPiece(m_selected_square.value());
    }
    m_selected_square = std::nullopt;
    m_legal_moves.clear();
}

auto ChessboardController::compute_piece_moves(chesscore::Square square) -> void {
    m_legal_moves.clear();
    const auto all_legal_moves = m_current_position.all_legal_moves();
    for (const auto &move : all_legal_moves) {
        if (move.from == square) {
            m_legal_moves.push_back(move);
            if (m_mark_target_squares) {
                m_board_widget->markSquare(move.to);
            }
        }
    }
}

auto ChessboardController::make_move(const chesscore::Move &move) -> void {
    m_current_position.make_move(move);
    m_board_widget->showPosition(m_current_position);
}

auto ChessboardController::set_position(const chesscore::Position &position) -> void {
    m_current_position = position;
    m_board_widget->showPosition(m_current_position);
}

} // namespace chessgui
