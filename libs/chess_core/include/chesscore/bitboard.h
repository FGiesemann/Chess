/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_BITBOARD_H
#define CHESSCORE_BITBOARD_H

#include <array>
#include <cstdint>

#include "chesscore/bitmap.h"
#include "chesscore/board.h"
#include "chesscore/fen.h"
#include "chesscore/move.h"
#include "chesscore/piece.h"
#include "chesscore/square.h"

namespace chesscore {

/**
 * \brief A bitboard stores the placement of pieces on the board.
 *
 * The bitboard contains Bitmaps that describe the placement of figures on the
 * chess board.
 */
class Bitboard {
public:
    Bitboard() = default;

    /**
     * \brief Create a board with pieces described by a FEN string.
     *
     * Sets the pieces on the board according to the figure placement part of a
     * FEN string.
     * \param fen The FEN string.
     */
    explicit Bitboard(const FenString &fen);

    /**
     * \brief Check if the bitboard is empty.
     *
     * An empty bitboard has no pieces on it.
     * \return If the bitboard is empty.
     */
    auto empty() const -> bool;

    /**
     * \brief Check if the bitboard has a piece of a certain type.
     *
     * Checks, if there is a piece of the given piece type on the board. For
     * checking a piece with a certain color, use the has_piece(Color)
     * \param piece_type The piece type.
     * \return If there is a piece of the given type on the board.
     */
    auto has_piece(const PieceType &piece_type) const -> bool;

    /**
     * \brief Check if the bitboard has a piece of a certain color.
     *
     * Checks, if there is a piece of the given color on the board. For only
     * checking the piece type ignoring the color, use the has_piece(PieceType).
     * \param piece The piece to check.
     * \return If there is a piece of the given color on the board.
     */
    auto has_piece(const Piece &piece) const -> bool;

    /**
     * \brief Check if the bitboard has a piece of a certain color.
     *
     * Checks if any piece of the given color is on the board.
     * \param color The color to check.
     * \return If there is a piece of the given color on the board.
     */
    auto has_piece(const Color &color) const -> bool;

    /**
     * \brief Check if the bitboard has a piece on a certain square.
     *
     * Checks a given suqare for a piece.
     * \param square The square to check.
     * \return If there is a piece on the given square.
     */
    auto has_piece(const Square &square) const -> bool;

    /**
     * \brief Put a piece on the board.
     *
     * Places the given piece on the given square. If the square is already
     * occupied, the new piece replaces the previous one.
     * \param piece The piece to place.
     * \param square The square to place the piece on.
     */
    auto set_piece(const Piece &piece, const Square &square) -> void;

    /**
     * \brief Get the piece on the given square.
     *
     * Retrieves the piece on the given square. If there is no piece on the
     * square, an empty optional is returned.
     * \param square The square to get the piece from.
     * \return The piece on the square or an empty optional.
     */
    auto get_piece(const Square &square) const -> std::optional<Piece>;

    /**
     * \brief Remove a piece from the board.
     *
     * Removes a piece from the given square. If there is no piece on the given
     * square, nothing happens.
     * \param square The square to clear.
     */
    auto clear_square(const Square &square) -> void;

    /**
     * \brief Make a move.
     *
     * Assumes, that the given move is valid in the current position. No checks
     * are performed!
     * \param move The move to apply in the current position.
     */
    auto make_move(const Move &move) -> void;

    /**
     * \brief Reverses a move.
     *
     * Recreates the state of the position before the move was made. This only
     * works, if the given move was the last move applied to the board. No
     * validity checks are performed!
     * \param move The move to undo.
     */
    auto unmake_move(const Move &move) -> void;

    /**
     * \brief Generate all legal moves.
     *
     * Generate a list of all legal moves for the current position. The function
     * needs additional information of the current position, such as the player
     * to move next, the available castling rights and the en-passant target
     * square.
     * \param state State of the current position.
     * \return A list of all legal moves for the given position and player.
     */
    auto all_legal_moves(const PositionState &state) const -> MoveList;

    /**
     * \brief Generate all moves for all knights.
     *
     * Generates the possible moves for the knights of the player to move.
     * \param moves The list, where the generated moves are added.
     * \param state State of the current position.
     */
    auto all_knight_moves(MoveList &moves, const PositionState &state) const -> void;

    /**
     * \brief Generate all moves for all kings.
     *
     * Generates the possible moves for the king(s) of the player to move.
     * \param moves The list, where the generated moves are added.
     * \param state State of the current position.
     */
    auto all_king_moves(MoveList &moves, const PositionState &state) const -> void;

    /**
     * \brief Generate all moves for a sliding piece.
     *
     * Generates the possible moves for a given sliding piece from the given
     * starting square in te given position state.
     * \param moving_piece The moving piece.
     * \param start The square, where the piece starts.
     * \param moves The list, where the generated moves are added.
     * \param state State of the current position.
     */
    auto all_sliding_moves(const Piece &moving_piece, const Square &start, MoveList &moves, const PositionState &state) const -> void;

    /**
     * \brief Generate all moves along a single direction.
     *
     * Generates the moves for a sliding piece along a single sliding direction.
     * \param moving_piece The moving piece.
     * \param start Starting suqare of the piece.
     * \param direction The ray direction.
     * \param moves The list, where the generated moves are added.
     * \param state State of the current position.
     */
    auto all_moves_along_ray(const Piece &moving_piece, const Square &start, const RayDirection &direction, MoveList &moves, const PositionState &state) const -> void;

    /**
     * \brief Generate all pawn moves for a player.
     *
     * Generates all the moves for all pawns of one player. This includes
     *   - single step pawn pushes (including promotion),
     *   - double step pawn pushes,
     *   - captures by pawns (including en-passant captures)
     * \param moves The list, where the generated moves are added.
     * \param state State of the current position.
     */
    auto all_pawn_moves(MoveList &moves, const PositionState &state) const -> void;

    /**
     * \brief Check, if a square is under attack.
     *
     * Checks, if the given square is under attack by a piece of the gven color.
     * \param square The square to be checked.
     * \param attacker_color Color of the attacker.
     * \return If the square is under attack.
     */
    auto is_attacked(const Square &square, Color attacker_color) const -> bool;

    /**
     * \brief Check, if a square would be under attack after a move.
     *
     * Checks, if the given square woudl be under attack by a piece of the given
     * color after the given move was applied.
     * \param square The square to check.
     * \param attacker_color Color or the attacker.
     * \param move The move to apply before the check.
     * \return If the square would be under attack after the move.
     */
    auto would_be_attacked(const Square &square, Color attacker_color, const Move &move) const -> bool;

    /**
     * \brief Checks, if a square is attacked by a pawn.
     *
     * \param square The square to be checked.
     * \param pawn_color Color of the attacking pawn.
     * \return If the square is attacked by a pawn.
     */
    auto pawn_attacks(const Square &square, Color pawn_color) const -> bool;

    /**
     * \brief Checks, if a square is attacked by a knight.
     *
     * \param square The square to be checked.
     * \param knight_color Color of the attacking knight.
     * \return If the square is attacked by a knight.
     */
    auto knight_attacks(const Square &square, Color knight_color) const -> bool;

    /**
     * \brief Checks, if a square is attacked by a king.
     *
     * \param square The square to be checked.
     * \param king_color Color of the attacking king.
     * \return If the square is attacked by a king.
     */
    auto king_attacks(const Square &square, Color king_color) const -> bool;

    /**
     * \brief Checks, if a square is attacked by a sliding piece.
     *
     * \param square The square to be checked.
     * \param piece_color Color of the attacking sliding piece.
     * \return If the square is attacked by a sliding piece.
     */
    auto sliding_piece_attacks(const Square &square, Color piece_color) const -> bool;
private:
    std::array<Bitmap, 12> m_bitmaps{};
    Bitmap m_white_pieces{};
    Bitmap m_black_pieces{};
    Bitmap m_all_pieces{};

    enum class PawnCaptureDirection { West, East };

    auto bitmap_index(const Piece &piece) const -> size_t {
        const auto type_index = static_cast<unsigned int>(piece.type);
        const auto color_offset = (piece.color == Color::White) ? 0U : 6U;
        return type_index + color_offset;
    }

    auto bitmap(const Piece &piece) const -> const Bitmap & { return m_bitmaps[bitmap_index(piece)]; }
    auto bitmap(const Piece &piece) -> Bitmap & { return m_bitmaps[bitmap_index(piece)]; }
    auto bitmap(const Color &color) const -> const Bitmap & { return color == Color::White ? m_white_pieces : m_black_pieces; }
    auto bitmap(const Color &color) -> Bitmap & { return color == Color::White ? m_white_pieces : m_black_pieces; }

    void move_castling_rook(const Move &move);
    void reset_castling_rook(const Move &move);

    auto remove_occupied_squares(const Bitmap &bitmap) const -> Bitmap;

    auto all_stepping_moves(PieceType piece_type, MoveList &moves, const PositionState &state) const -> void;
    auto all_targets_along_ray(const Square &start, Color moving_color, const RayDirection &direction) const -> Bitmap;
    auto attacked_from_ray(const Square &square, Color piece_color, RayDirection direction, PieceType attacker1, PieceType attacker2) const -> bool;

    auto extract_moves(Bitmap targets, const Square &from, const Piece &piece, const PositionState &state, MoveList &moves) const -> void;
    static auto extract_pawn_moves(Bitmap targets, int step_size, const PositionState &state, MoveList &moves) -> void;
    auto extract_pawn_captures(Bitmap targets, PawnCaptureDirection direction, const PositionState &state, MoveList &moves) const -> void;
};

static_assert(Board<Bitboard>, "Bitboard should implement the Board concept");

} // namespace chesscore

#endif
