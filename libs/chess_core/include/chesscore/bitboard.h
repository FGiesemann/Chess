/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#ifndef CHESSCORE_BITBOARD_H
#define CHESSCORE_BITBOARD_H

#include <cstdint>

#include "chesscore/square.h"

namespace chesscore {

class Bitmap {
public:
    Bitmap() = default;
    explicit constexpr Bitmap(const Square &square) { set(square); }
    explicit constexpr Bitmap(const std::uint64_t &bits) : m_bits{bits} {}

    constexpr auto empty() const -> bool { return m_bits == 0ull; }

    constexpr auto get(const Square &square) const -> bool { return m_bits & bit_mask(square.index()); }
    constexpr auto set(const Square &square) -> void { m_bits |= bit_mask(square.index()); }
    constexpr auto clear(const Square &square) -> void { m_bits &= ~bit_mask(square.index()); }
    constexpr auto toggle(const Square &square) -> void { m_bits ^= bit_mask(square.index()); }

    friend constexpr auto operator==(const Bitmap &lhs, const Bitmap &rhs) -> bool { return lhs.m_bits == rhs.m_bits; }
    constexpr auto operator&=(const Bitmap &rhs) -> Bitmap & {
        m_bits &= rhs.m_bits;
        return *this;
    }
    constexpr auto operator|=(const Bitmap &rhs) -> Bitmap & {
        m_bits |= rhs.m_bits;
        return *this;
    }
    constexpr auto operator^=(const Bitmap &rhs) -> Bitmap & {
        m_bits ^= rhs.m_bits;
        return *this;
    }

    constexpr auto bits() const -> std::uint64_t { return m_bits; }
private:
    std::uint64_t m_bits{};

    static constexpr auto bit_mask(int index) -> std::uint64_t { return 1ull << (63 - index); }
};

constexpr auto operator|(const Bitmap &lhs, const Bitmap &rhs) -> Bitmap {
    Bitmap result{lhs};
    result |= rhs;
    return result;
}
constexpr auto operator&(const Bitmap &lhs, const Bitmap &rhs) -> Bitmap {
    Bitmap result{lhs};
    result &= rhs;
    return result;
}
constexpr auto operator^(const Bitmap &lhs, const Bitmap &rhs) -> Bitmap {
    Bitmap result{lhs};
    result ^= rhs;
    return result;
}
constexpr auto operator~(const Bitmap &bitmap) -> Bitmap {
    Bitmap result{~bitmap.bits()};
    return result;
}

} // namespace chesscore

#endif
