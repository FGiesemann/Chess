/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <cstdint>
#include <iostream>

#include "bigmap.h"

namespace chesscore::table_gen {

Bigmap::Bigmap(const Pattern &pattern) {
    for (std::size_t row = 0; row < pattern_width; ++row) {
        for (std::size_t col = 0; col < pattern_width; ++col) {
            const std::size_t pattern_index{row * pattern_width + col};
            if (pattern[pattern_index]) {
                m_bits[index(row, col)] = true;
            }
        }
    }
}

auto Bigmap::shift(ShiftDirection dir, std::size_t steps) -> void {
    switch (dir) {
    case ShiftDirection::North:
        m_bits <<= bigmap_width * steps;
        break;
    case ShiftDirection::South:
        m_bits >>= bigmap_width * steps;
        break;
    case ShiftDirection::East:
        m_bits <<= steps;
        break;
    case ShiftDirection::West:
        m_bits >>= steps;
        break;
    default:
        return;
    }
}

auto Bigmap::extract_board() const -> Bitmap {
    std::uint64_t bits{};

    for (std::size_t row = 2 * overhang; row >= overhang; --row) {
        for (std::size_t col = 2 * overhang; col >= overhang; --col) {
            bits = (bits << 1) | (m_bits[index(row, col)] ? 1 : 0);
        }
    }

    return Bitmap{bits};
}

auto Bigmap::inside_board(std::size_t row, std::size_t col) -> bool {
    return row >= overhang && row <= 2 * overhang && col >= overhang && col <= 2 * overhang;
}

auto operator<<(std::ostream &os, const Bigmap &bigmap) -> std::ostream & {
    for (std::size_t row = bigmap_width; row > 0; --row) {
        for (std::size_t col = 0; col < bigmap_width; ++col) {
            if (bigmap.at(row - 1, col)) {
                os << (Bigmap::inside_board(row - 1, col) ? 'X' : 'x');
            } else {
                os << (Bigmap::inside_board(row - 1, col) ? "○" : "·");
            }
            os << ' ';
        }
        os << '\n';
    }
    return os;
}

} // namespace chesscore::table_gen
