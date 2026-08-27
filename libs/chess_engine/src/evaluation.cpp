/* ************************************************************************** *
 * Chess Engine Maat                                                          *
 * Chess playing engine                                                       *
 * ************************************************************************** */

#include "chess_engine/evaluation.h"

namespace chess_engine {

auto Evaluator::evaluate(const chess_core::Position &position, chess_core::Color color) const -> Score {
    if (is_mate(position)) {
        return color == position.side_to_move() ? -Score::Mate : Score::Mate;
    }
    Score score{0};
    if (m_config.use_material_balance) {
        score += countup_material(position, color) - countup_material(position, chess_core::other_color(color));
    }
    if (m_config.use_piece_square_tables) {
        score += evaluate_pieces_on_squares(position, color);
    }
    return score;
}

auto Evaluator::evaluate(const chess_core::Move &move) const -> Score {
    Score score{0};
    if (m_config.use_capture_bonus) {
        score += get_capture_score(move);
    }
    if (m_config.use_promotion_bonus) {
        score += get_promotion_score(move);
    }
    if (m_config.use_piece_square_tables) {
        score += get_piece_movement_score(move);
    }
    return score;
}

auto Evaluator::is_mate(const chess_core::Position &position) -> bool {
    return position.check_state() == chess_core::CheckState::Checkmate;
}

auto Evaluator::countup_material(const chess_core::Position &position, chess_core::Color color) const -> Score {
    Score material{0};
    for (const auto piece_type : chess_core::all_piece_types) {
        material += m_config.piece_value(piece_type) * position.board().piece_count(chess_core::Piece{piece_type, color});
    }
    return material;
}

auto Evaluator::evaluate_pieces_on_squares(const chess_core::Position &position, chess_core::Color color) const -> Score {
    Score score{0};
    chess_core::Square square{chess_core::Square::A1};
    for (int i = 0; i < chess_core::Square::count; ++i) {
        const auto piece = position.board().get_piece(square);
        if (piece.has_value() && piece->color() == color) {
            score += m_config.piece_on_square_value(piece.value(), square);
        }
        square += 1;
    }
    return score;
}

auto Evaluator::get_capture_score(const chess_core::Move &move) const -> Score {
    if (move.is_capture()) {
        return m_config.piece_value(move.captured.value().type());
    }
    return Score{0};
}

auto Evaluator::get_promotion_score(const chess_core::Move &move) const -> Score {
    if (move.is_pawn_promotion()) {
        return m_config.pawn_promotion_score + m_config.piece_value(move.promoted.value().type()) - m_config.piece_value(chess_core::PieceType::Pawn);
    }
    return Score{0};
}

auto Evaluator::get_piece_movement_score(const chess_core::Move &move) const -> Score {
    return m_config.piece_on_square_value(move.piece, move.to) - m_config.piece_on_square_value(move.piece, move.from);
}

} // namespace chess_engine
