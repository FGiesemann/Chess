/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/position.h"

namespace chesscore {

auto Position::make_move(const Move &move) -> void {
    update_piece_hash(move);
    m_board.make_move(move);
    updateFullmoveNumber();
    updateHalfmoveClock(move);
    updateEnPassant(move);
    updateCastlingRights(move);
    m_state.side_to_move = other_color(m_state.side_to_move);
    m_hash.swap_side();
}

auto Position::update_piece_hash(const Move &move) -> void {
    if (move.is_capture()) {
        m_hash.clear_piece(move.captured.value(), move.to);
    }
    m_hash.move_piece(move.piece, move.from, move.to);
}

auto Position::updateFullmoveNumber() -> void {
    if (m_state.side_to_move == Color::Black) {
        m_state.fullmove_number++;
    }
}

auto Position::updateHalfmoveClock(const Move &move) -> void {
    if (move.is_capture() || move.piece.type == PieceType::Pawn) {
        m_state.halfmove_clock = 0;
    } else {
        m_state.halfmove_clock++;
    }
}

auto Position::updateEnPassant(const Move &move) -> void {
    if (move.piece.type == PieceType::Pawn && move.is_double_step()) {
        if (move.from.rank().rank > move.to.rank().rank) {
            m_state.en_passant_target = Square{File{move.from.file().file}, Rank{move.from.rank().rank - 1}};
        } else {
            m_state.en_passant_target = Square{File{move.from.file().file}, Rank{move.from.rank().rank + 1}};
        }
        m_hash.set_enpassant(move.from.file());
    } else {
        m_state.en_passant_target.reset();
    }
}

auto Position::updateCastlingRights(const Move &move) -> void {
    if (move.piece == Piece::WhiteKing) {
        m_state.castling_rights['K'] = false;
        m_state.castling_rights['Q'] = false;
    } else if (move.piece == Piece::WhiteRook) {
        if (move.from == Square::H1) {
            m_state.castling_rights['K'] = false;
        } else if (move.from == Square::A1) {
            m_state.castling_rights['Q'] = false;
        }
    } else if (move.piece == Piece::BlackKing) {
        m_state.castling_rights['k'] = false;
        m_state.castling_rights['q'] = false;
    } else if (move.piece == Piece::BlackRook) {
        if (move.from == Square::H8) {
            m_state.castling_rights['k'] = false;
        } else if (move.from == Square::A8) {
            m_state.castling_rights['q'] = false;
        }
    }
    if (move.is_capture()) {
        if (move.to == Square::A1) {
            m_state.castling_rights['Q'] = false;
        } else if (move.to == Square::H1) {
            m_state.castling_rights['K'] = false;
        } else if (move.to == Square::A8) {
            m_state.castling_rights['q'] = false;
        } else if (move.to == Square::H8) {
            m_state.castling_rights['k'] = false;
        }
    }
}

auto Position::unmake_move(const Move &move) -> void {
    m_board.unmake_move(move);
    resetFullmoveNumber(move);
    resetHalfmoveClock(move);
    resetEnPassant(move);
    resetCastlingRights(move);
    m_state.side_to_move = other_color(m_state.side_to_move);
}

auto Position::resetFullmoveNumber(const Move &move) -> void {
    if (move.piece.color == Color::Black) {
        m_state.fullmove_number--;
    }
}

auto Position::resetHalfmoveClock(const Move &move) -> void {
    m_state.halfmove_clock = move.halfmove_clock_before;
}

auto Position::resetEnPassant(const Move &move) -> void {
    if (move.en_passant_target_before.has_value()) {
        m_state.en_passant_target = move.en_passant_target_before;
    } else {
        m_state.en_passant_target.reset();
    }
}

auto Position::resetCastlingRights(const Move &move) -> void {
    m_state.castling_rights = move.castling_rights_before;
}

auto Position::all_legal_moves() const -> MoveList {
    return m_board.all_legal_moves(state());
}

auto Position::capture_moves() const -> MoveList {
    return m_board.capture_moves(state());
}

auto Position::is_king_in_check(Color color) const -> bool {
    const auto king_sq = m_board.find_king(color);
    if (king_sq.has_value()) {
        return m_board.is_attacked(king_sq.value(), other_color(color));
    }
    return false;
}

auto Position::check_state() const -> CheckState {
    if (is_king_in_check(m_state.side_to_move)) {
        if (all_legal_moves().empty()) {
            return CheckState::Checkmate;
        } else {
            return CheckState::Check;
        }
    } else {
        if (all_legal_moves().empty()) {
            return CheckState::Stalemate;
        }
    }
    return CheckState::None;
}

auto Position::piece_placement() const -> PiecePlacement {
    PiecePlacement pieces{};
    for (int rank = Rank::max_rank; rank >= Rank::min_rank; --rank) {
        for (int file = File::min_file; file <= File::max_file; ++file) {
            const Square square{file, rank};
            const auto piece = m_board.get_piece(square);
            if (piece) {
                pieces[square.index()] = piece.value();
            }
        }
    }
    return pieces;
}

auto Position::operator==(const Position &rhs) const -> bool {
    return m_board == rhs.m_board && m_state == rhs.m_state;
}

} // namespace chesscore
