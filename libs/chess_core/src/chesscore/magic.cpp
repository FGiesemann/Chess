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

auto attack_bitmap(PieceType piece_type, const Square &square, Bitmap blocker_config) -> Bitmap {
    if (piece_type != PieceType::Rook && piece_type != PieceType::Bishop) {
        return Bitmap{};
    }

    Bitmap attack_map{};
    const auto ray_directions_for_piece = piece_ray_directions[piece_type];
    for (const auto direction : all_ray_directions) {
        if (ray_directions_for_piece & direction) {
            auto targets = bitmaps::ray_target_table[direction][square];
            const auto blockers = targets & blocker_config;
            if (!blockers.empty()) {
                const auto blocker_square = Square::A1 + (is_negative_direction(direction) ? 63 - blockers.empty_squares_after() : blockers.empty_squares_before());
                targets ^= bitmaps::ray_target_table[direction][blocker_square];
            }
            attack_map |= targets;
        }
    }
    return attack_map;
}

} // namespace chesscore
