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
    auto empty() const -> bool { return m_bits == 0ull; }

    auto get(const Square &square) const -> bool { return m_bits & bit_mask(square.index()); }
    auto set(const Square &square) -> void { m_bits |= bit_mask(square.index()); }
    auto clear(const Square &square) -> void { m_bits &= ~bit_mask(square.index()); }
    auto toggle(const Square &square) -> void { m_bits ^= bit_mask(square.index()); }
private:
    std::uint64_t m_bits{};

    static auto bit_mask(int index) -> std::uint64_t { return 1ull << (63 - index); }
};

} // namespace chesscore

#endif
