/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#ifndef CHESSCORE_TABLE_GEN_BIGMAP_H
#define CHESSCORE_TABLE_GEN_BIGMAP_H

#include <bitset>
#include <iosfwd>

#include "chesscore/bitmap.h"
#include "dimensions.h"
#include "patterns.h"

namespace chesscore::table_gen {

enum class ShiftDirection { North, South, East, West };

class Bigmap {
public:
    Bigmap() = default;
    Bigmap(const Pattern &pattern);

    using Bitset = std::bitset<bigmap_width * bigmap_width>;

    auto operator[](std::size_t pos) const -> bool { return m_bits[pos]; }
    auto at(std::size_t row, std::size_t col) const -> bool { return m_bits[index(row, col)]; }
    auto operator[](std::size_t pos) -> Bitset::reference { return m_bits[pos]; }

    auto shift(ShiftDirection dir, std::size_t steps = 1U) -> void;

    auto extract_board() const -> Bitmap;
    auto index(std::size_t row, std::size_t col) const -> std::size_t { return row * bigmap_width + col; }
    static auto inside_board(std::size_t row, std::size_t col) -> bool;
private:
    Bitset m_bits{};
};

auto operator<<(std::ostream &os, const Bigmap &bigmap) -> std::ostream &;

} // namespace chesscore::table_gen

#endif
