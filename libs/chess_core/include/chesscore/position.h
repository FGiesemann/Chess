/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_POSITION_H
#define CHESSCORE_POSITION_H

#include "chesscore/board.h"
#include "chesscore/fen.h"
#include "chesscore/position_types.h"

namespace chesscore {

/**
 * \brief The current state of a chess game.
 *
 * Represents the state of a game of chess. It includes the current placement of
 * pieces on the board, the current turn, castling rights, the playerr to move,
 * and many other pieces of information that describe the state of the game.
 * \tparam BoardT The board representation that manages the pieces on the board.
 */
template<Board BoardT>
class Position {
public:
    /**
     * \brief Create a new Position.
     *
     * The position holds an empty board, no castling rights and white to play.
     */
    Position() = default;

    /**
     * \brief Create a Position from a FEN string.
     *
     * The position is set up according to the FEN string.
     * \param fen The FEN string.
     */
    explicit Position(const FenString &fen)
        : m_board{fen}, m_state{
                            .side_to_move{fen.side_to_move()},
                            .fullmove_number{fen.fullmove_number()},
                            .halfmove_clock{fen.halfmove_clock()},
                            .castling_rights{fen.castling_rights()},
                            .en_passant_target{fen.en_passant_square()}
                        } {}

    /**
     * \brief Access the board representation.
     *
     * The board representation manages the pieces on the board.
     * \return The board representation.
     */
    auto board() const -> const BoardT & { return m_board; }

    /**
     * \brief Return the player to move next.
     *
     * \return The player to move next.
     */
    auto side_to_move() const -> Color { return m_state.side_to_move; }

    /**
     * \brief Return the fullmove number.
     *
     * The fullmove number is the number of the next move. In this context, a
     * move consists of the half-moves of White and then Black. The fullmove
     * number increases, after Black plays.
     * \return The fullmove number.
     */
    auto fullmove_number() const -> int { return m_state.fullmove_number; }

    /**
     * \brief The halfmove clock.
     *
     * \return The halfmove clock.
     */
    auto halfmove_clock() const -> int { return m_state.halfmove_clock; }

    /**
     * \brief The current castling rights.
     *
     * Returns the rights to castle for each player.
     * \return The castling rights
     */
    auto castling_rights() const -> CastlingRights { return m_state.castling_rights; }

    /**
     * \brief An optional en-passant target square.
     *
     * If the last move was a double-step of a pawn, the overstepped square can
     * be the target of capturing en-passant in the following move. In that
     * case, this square is returned. If no capture by en-passant is possible,
     * this is an empty optional.
     * \return The possible en-passant target square.
     */
    auto en_passant_target() const -> std::optional<Square> { return m_state.en_passant_target; }

    /**
     * \brief Perform a move.
     *
     * Applies the given move in the current position. The move is assumed to be
     * valid in the current position. No validity cheks are performed!
     * \param move The move to apply.
     */
    auto make_move(const Move &move) -> void;

    /**
     * \brief Undo a move.
     *
     * Reverts the position back to the state before the move was made. This
     * only works, if the move was the last move made in this position. No
     * validity checks are performed!
     * \param move The move to undo.
     */
    auto unmake_move(const Move &move) -> void;

    /**
     * \brief The current state of the position.
     *
     * \return State of the position.
     */
    auto state() const -> const PositionState & { return m_state; }

    /**
     * \brief Generate all legal moves.
     *
     * Generate a list of all legal moves from the current position for the
     * player to move.
     * \return A list of all legal moves for the given position.
     */
    auto all_legal_moves() const -> MoveList;

    /**
     * \brief Checks, if a king is under attack.
     *
     * Checks, if the king of the given color is under attack. If no king of the
     * given color exists, it is obviously not under attack.
     * \param color The color
     * \return If the king of the given color is under attack.
     */
    auto is_king_in_check(Color color) const -> bool;

    /**
     * \brief Determine the check state of the position.
     *
     * The check state of the player to move is returned. The player is in
     * check, if his king is under attack, but he still has legal moves. If
     * there are no legal moves, the player is in checkmate.
     * \return Check state for the player to move.
     */
    auto check_state() const -> CheckState;

    /**
     * \brief Get the piece placement of the position.
     *
     * Creates the piece placement describing the pieces on the board.
     * \return The piece placement.
     */
    auto piece_placement() const -> PiecePlacement;
private:
    BoardT m_board{};        ///< Current placement of pieces on the board.
    PositionState m_state{}; ///< The current state of the position.

    auto updateCastlingRights(const Move &move) -> void;
    auto updateFullmoveNumber() -> void;
    auto updateHalfmoveClock(const Move &move) -> void;
    auto updateEnPassant(const Move &move) -> void;
    auto resetFullmoveNumber(const Move &move) -> void;
    auto resetHalfmoveClock(const Move &move) -> void;
    auto resetEnPassant(const Move &move) -> void;
    auto resetCastlingRights(const Move &move) -> void;
};

template<Board BoardT>
auto Position<BoardT>::make_move(const Move &move) -> void {
    m_board.make_move(move);
    updateFullmoveNumber();
    updateHalfmoveClock(move);
    updateEnPassant(move);
    updateCastlingRights(move);
    m_state.side_to_move = other_color(m_state.side_to_move);
}

template<Board BoardT>
void Position<BoardT>::updateFullmoveNumber() {
    if (m_state.side_to_move == Color::Black) {
        m_state.fullmove_number++;
    }
}

template<Board BoardT>
void Position<BoardT>::updateHalfmoveClock(const Move &move) {
    if (move.is_capture() || move.piece.type == PieceType::Pawn) {
        m_state.halfmove_clock = 0;
    } else {
        m_state.halfmove_clock++;
    }
}

template<Board BoardT>
void Position<BoardT>::updateEnPassant(const Move &move) {
    if (move.piece.type == PieceType::Pawn && move.is_double_step()) {
        if (move.from.rank().rank > move.to.rank().rank) {
            m_state.en_passant_target = Square{File{move.from.file().file}, Rank{move.from.rank().rank - 1}};
        } else {
            m_state.en_passant_target = Square{File{move.from.file().file}, Rank{move.from.rank().rank + 1}};
        }
    } else {
        m_state.en_passant_target.reset();
    }
}

template<Board BoardT>
void Position<BoardT>::updateCastlingRights(const Move &move) {
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

template<Board BoardT>
auto Position<BoardT>::unmake_move(const Move &move) -> void {
    m_board.unmake_move(move);
    resetFullmoveNumber(move);
    resetHalfmoveClock(move);
    resetEnPassant(move);
    resetCastlingRights(move);
    m_state.side_to_move = other_color(m_state.side_to_move);
}

template<Board BoardT>
auto Position<BoardT>::resetFullmoveNumber(const Move &move) -> void {
    if (move.piece.color == Color::Black) {
        m_state.fullmove_number--;
    }
}

template<Board BoardT>
auto Position<BoardT>::resetHalfmoveClock(const Move &move) -> void {
    m_state.halfmove_clock = move.halfmove_clock_before;
}

template<Board BoardT>
auto Position<BoardT>::resetEnPassant(const Move &move) -> void {
    if (move.en_passant_target_before.has_value()) {
        m_state.en_passant_target = move.en_passant_target_before;
    } else {
        m_state.en_passant_target.reset();
    }
}

template<Board BoardT>
auto Position<BoardT>::resetCastlingRights(const Move &move) -> void {
    m_state.castling_rights = move.castling_rights_before;
}

template<Board BoardT>
auto Position<BoardT>::all_legal_moves() const -> MoveList {
    return m_board.all_legal_moves(state());
}

template<Board BoardT>
auto Position<BoardT>::is_king_in_check(Color color) const -> bool {
    const auto king_sq = m_board.find_king(color);
    if (king_sq.has_value()) {
        return m_board.is_attacked(king_sq.value(), other_color(color));
    }
    return false;
}

template<Board BoardT>
auto Position<BoardT>::check_state() const -> CheckState {
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

template<Board BoardT>
auto Position<BoardT>::piece_placement() const -> PiecePlacement {
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

} // namespace chesscore

#endif
