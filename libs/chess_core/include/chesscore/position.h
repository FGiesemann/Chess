/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_POSITION_H
#define CHESSCORE_POSITION_H

#include "chesscore/bitboard.h"
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
 */
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
     * \brief Create the starting position.
     *
     * \return The starting position.
     */
    static auto start_position() -> Position { return Position{FenString::starting_position()}; }

    /**
     * \brief Access the board representation.
     *
     * The board representation manages the pieces on the board.
     * \return The board representation.
     */
    auto board() const -> const Bitboard & { return m_board; }

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
     * \brief Generate all (legal) capture moves.
     *
     * Generate a list of all legal capture moves from the current position for
     * the player to move.
     * \return A list of all legal capture moves for the given position.
     */
    auto capture_moves() const -> MoveList;

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

    /**
     * \brief Comparison of two positions.
     *
     * \param rhs The position to compare to.
     * \return If the positions are equal.
     */
    auto operator==(const Position &rhs) const -> bool;
private:
    Bitboard m_board{};      ///< Current placement of pieces on the board.
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

} // namespace chesscore

#endif
