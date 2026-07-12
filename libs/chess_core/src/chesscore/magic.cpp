/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/magic.h"
#include "chesscore/bitboard_tables.h"

namespace chesscore {

auto blocker_mask(PieceType piece_type, const Square &square) -> Bitmap {
    if (piece_type == PieceType::Bishop) {
        return bitmaps::bishop_target_table[square] & ~bitmaps::board_border;
    }
    if (piece_type == PieceType::Rook) {
        return ((bitmaps::file_table[square.file()] & ~bitmaps::rank_table[Rank{Rank::min_rank}] & ~bitmaps::rank_table[Rank{Rank::max_rank}]) |
                (bitmaps::rank_table[square.rank()] & ~bitmaps::file_table[File{File::min_file}] & ~bitmaps::file_table[File{File::max_file}])) &
               ~Bitmap{square};
    }
    return Bitmap{};
}

auto next_blocker_config(Bitmap blockers, Bitmap mask) -> Bitmap {
    return Bitmap{blockers.bits() - mask.bits()} & mask;
}

} // namespace chesscore
