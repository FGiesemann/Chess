/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "bigmap.h"
#include <iostream>

namespace chesscore::table_gen {

auto Bigmap::extract_board() const -> Bitmap {
    return {};
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
