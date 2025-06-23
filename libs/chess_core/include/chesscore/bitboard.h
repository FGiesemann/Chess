/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#ifndef CHESSCORE_BITBOARD_H
#define CHESSCORE_BITBOARD_H

#include <cstdint>

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

    static constexpr auto bit_mask(int index) -> std::uint64_t { return 1ULL << (63 - index); }
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

} // namespace chesscore

#endif
