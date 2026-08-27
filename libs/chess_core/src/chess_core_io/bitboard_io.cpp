/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chess_core_io/bitboard_io.h"
#include "chess_core_io/piece_io.h"

#include <iostream>

namespace chess_core {

auto operator<<(std::ostream &os, const Bitboard &bitboard) -> std::ostream & {
    os << "  a b c d e f g h\n";
    for (int rank = chess_core::Rank::count - 1; rank >= 0; --rank) {
        os << (rank + 1) << ' ';
        for (int file = 0; file < chess_core::File::count; ++file) {
            const chess_core::Square square{file, rank};
            const auto piece = bitboard.get_piece(square);
            if (piece) {
                os << piece.value();
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

} // namespace chess_core
