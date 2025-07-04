/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "table_gen.h"
#include "chesscore_io/bitmap_io.h"

#include <iostream>

namespace chesscore::table_gen {

auto generate_table(const std::string &name, const Pattern &pattern, std::ostream &os) -> void {
    const char row_name[] = "12345678";
    const char col_name[] = "ABCDEFGH";
    Bigmap bigmap{pattern};
    os << "constexpr TargetTable " << name << "_target_table{\n";

    for (std::size_t row = 0; row < board_width; ++row) {
        for (std::size_t col = 0; col < board_width; ++col) {
            os << "    Bitmap{" << as_ull_hex(bigmap.extract_board()) << "}, // " << col_name[col] << row_name[row] << '\n';
            bigmap.shift(ShiftDirection::East);
        }
        bigmap.shift(ShiftDirection::West, 8);
        bigmap.shift(ShiftDirection::North);
    }

    os << "};\n";
}

} // namespace chesscore::table_gen
