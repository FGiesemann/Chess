/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_BITBOARD_H
#define CHESSCORE_BITBOARD_H

#include <array>
#include <cstdint>

#include "chesscore/board.h"
#include "chesscore/fen.h"
#include "chesscore/move.h"
#include "chesscore/piece.h"
#include "chesscore/square.h"

namespace chesscore {

/**
 * \brief A bit map describing a set of positions on a chess board.
 *
 * A bit map is a set of 64 bits that represent the 64 squares of a chess board.
 */
class Bitmap {
public:
    /**
     * \brief Create an empty bit map.
     *
     * In an empty bit map, no bits are set.
     */
    Bitmap() = default;

    /**
     * \brief Create a bit map with a single bit set.
     *
     * Creates an bit set with a single bit set at the position specified by the
     * square.
     * \param square The square whose corresponding bit will be set.
     */
    explicit constexpr Bitmap(const Square &square) { set(square); }

    /**
     * \brief Create a bit map with a given pattern.
     *
     * Create a bit map with the bits given in the bits parameter.
     * \param bits The bits to set in the bit map.
     */
    explicit constexpr Bitmap(const std::uint64_t &bits) : m_bits{bits} {}

    /**
     * \brief Check if the bit map is empty.
     *
     * An empty bit map has no bits set.
     * \return If the bitmap is empty.
     */
    constexpr auto empty() const -> bool { return m_bits == 0ull; }

    /**
     * \brief Query a bit in the bit map.
     *
     * Query the bit at the position specified by the square.
     * \param square The position to query.
     * \return If the bit at the given position is set.
     */
    constexpr auto get(const Square &square) const -> bool { return m_bits & bit_mask(square.index()); }

    /**
     * \brief Set a bit in the bit map.
     *
     * Set the bit at the position specified by the square.
     * \param square The position to set.
     */
    constexpr auto set(const Square &square) -> void { m_bits |= bit_mask(square.index()); }

    /**
     * \brief Clear a bit in the bit map.
     *
     * Sets the bit at the position specified by the square to 0.
     * \param square The Position to clear.
     */
    constexpr auto clear(const Square &square) -> void { m_bits &= ~bit_mask(square.index()); }

    /**
     * \brief Toggle a bit in the bit map.
     *
     * Switches the bit at the given position from 0 to 1 and vice versa.
     * \param square The position to toggle.
     */
    constexpr auto toggle(const Square &square) -> void { m_bits ^= bit_mask(square.index()); }

    /**
     * \brief Equality comparison.
     *
     * Bitmaps are equal if they have the same bits set.
     */
    friend constexpr auto operator==(const Bitmap &lhs, const Bitmap &rhs) -> bool { return lhs.m_bits == rhs.m_bits; }

    /**
     * \brief Bitwise and assignment.
     *
     * Modifies the bits in this bitmap by bitwise anding them with the bits in
     * the other bitmap.
     * \param rhs The other bitmap.
     * \return The modified bitmap.
     */
    constexpr auto operator&=(const Bitmap &rhs) -> Bitmap & {
        m_bits &= rhs.m_bits;
        return *this;
    }

    /**
     * \brief Bitwise or assignment.
     *
     * Modifies the bits in this bitmap by bitwise oring them with the bits in
     * the other bitmap.
     * \param rhs The other bitmap.
     * \return The modified bitmap.
     */
    constexpr auto operator|=(const Bitmap &rhs) -> Bitmap & {
        m_bits |= rhs.m_bits;
        return *this;
    }

    /**
     * \brief Bitwise xor assignment.
     *
     * Modifies the bits in this bitmap by bitwise xoring them with the bits in
     * the other bitmap.
     * \param rhs The other bitmap.
     * \return The modified bitmap.
     */
    constexpr auto operator^=(const Bitmap &rhs) -> Bitmap & {
        m_bits ^= rhs.m_bits;
        return *this;
    }

    /**
     * \brief Direct access to the unerlying bits.
     *
     * This gives the 64 bits in a 64-bit unsigned integer.
     * \return The bits in the bitmap.
     */
    constexpr auto bits() const -> std::uint64_t { return m_bits; }
private:
    std::uint64_t m_bits{};

    static constexpr auto bit_mask(size_t index) -> std::uint64_t { return 1ULL << (63U - index); }
};

/**
 * \brief Bitwise or combination of bitmaps.
 *
 * Generates a new bitmap with a bit set if at least one of the corresponding
 * bits in the two bitmaps is set.
 */
constexpr auto operator|(const Bitmap &lhs, const Bitmap &rhs) -> Bitmap {
    Bitmap result{lhs};
    result |= rhs;
    return result;
}

/**
 * \brief Bitwise and combination of bitmaps.
 *
 * Generates a new bitmap with a bit set if both the corresponding bits in the
 * two bitmaps are set.
 */
constexpr auto operator&(const Bitmap &lhs, const Bitmap &rhs) -> Bitmap {
    Bitmap result{lhs};
    result &= rhs;
    return result;
}

/**
 * \brief Bitwise xor combination of bitmaps.
 *
 * Generates a new bitmap with a bit set if the corresponding bits in the two
 * bitmaps are different.
 */
constexpr auto operator^(const Bitmap &lhs, const Bitmap &rhs) -> Bitmap {
    Bitmap result{lhs};
    result ^= rhs;
    return result;
}

/**
 * \brief Bitwise negation of a bitmap.
 *
 * Generates a new bitmap with a bit set if the corresponding bit in the
 * bitmap is not set.
 */
constexpr auto operator~(const Bitmap &bitmap) -> Bitmap {
    Bitmap result{~bitmap.bits()};
    return result;
}

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
    auto all_legal_moves(const PositionState &state) const -> std::vector<Move>;
private:
    std::array<Bitmap, 12> m_bitmaps{};
    Bitmap m_white_pieces{};
    Bitmap m_black_pieces{};
    Bitmap m_all_pieces{};

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
};

static_assert(Board<Bitboard>, "Bitboard should implement the Board concept");

} // namespace chesscore

#endif
