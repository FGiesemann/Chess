/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore_io/bitboard_io.h"
#include "chesscore_io/piece_io.h"

#include <iostream>

namespace chesscore {

auto operator<<(std::ostream &os, const Bitboard &bitboard) -> std::ostream & {
    os << "  a b c d e f g h\n";
    for (int rank = chesscore::Rank::max_rank; rank >= chesscore::Rank::min_rank; --rank) {
        os << rank << ' ';
        for (int file = chesscore::File::min_file; file <= chesscore::File::max_file; ++file) {
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
