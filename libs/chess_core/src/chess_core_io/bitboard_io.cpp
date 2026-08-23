/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chess_core_io/bitboard_io.h"
#include "chess_core_io/piece_io.h"

#include <iostream>

namespace chesscore {

auto operator<<(std::ostream &os, const Bitboard &bitboard) -> std::ostream & {
    os << "  a b c d e f g h\n";
    for (int rank = chesscore::Rank::count - 1; rank >= 0; --rank) {
        os << (rank + 1) << ' ';
        for (int file = 0; file < chesscore::File::count; ++file) {
            const chesscore::Square square{file, rank};
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

} // namespace chesscore
