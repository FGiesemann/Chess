/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore_io/bitmap_io.h"

#include <iomanip>
#include <iostream>
#include <sstream>

namespace chesscore {

auto operator<<(std::ostream &os, const Bitmap &bitmap) -> std::ostream & {
    os << "  a b c d e f g h\n";
    for (int rank = chesscore::Rank::max_rank; rank >= chesscore::Rank::min_rank; --rank) {
        os << rank << ' ';
        for (int file = chesscore::File::min_file; file <= chesscore::File::max_file; ++file) {
            const chesscore::Square square{file, rank};
            if (bitmap.get(square)) {
                os << 'X';
            } else {
                os << "·";
            }
            os << ' ';
        }
        os << rank << '\n';
    }
    os << "  a b c d e f g h\n";
    return os;
}

auto as_grouped_hex(const Bitmap &bitmap) -> std::string {
    std::stringstream oss;
    static constexpr int byte_count = 8;

    for (int i = 0; i < byte_count; ++i) {
        const auto byte = (bitmap.bits() >> (56 - i * 8)) & 0xff;
        if (i > 0) {
            oss << '\'';
        }
        oss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << byte;
    }

    return oss.str();
}

auto as_ull_hex(const Bitmap &bitmap) -> std::string {
    std::stringstream oss;
    oss << "0x" << std::hex << std::setfill('0') << std::setw(16) << std::uppercase << bitmap.bits() << "ULL";
    return oss.str();
}

auto as_grouped_bits(const Bitmap &bitmap) -> std::string {
    std::stringstream oss;

    static constexpr int square_count = 64;

    for (int i = 0; i < square_count; ++i) {
        if (i > 0 && i % chesscore::File::max_file == 0) {
            oss << '\'';
        }
        oss << ((bitmap.bits() >> (square_count - 1 - i)) & 1);
    }

    return oss.str();
}

} // namespace chesscore
