/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/magic.h"
#include "chesscore/bitboard_tables.h"

namespace chesscore {

auto blocker_mask(PieceType piece_type, Square square) -> Bitmap {
    if (piece_type == PieceType::Bishop) {
        return bitmaps::bishop_target_table[square] & ~bitmaps::board_border;
    }
    if (piece_type == PieceType::Rook) {
        return ((bitmaps::file_table[square.file()] & ~bitmaps::rank_table[Rank{0}] & ~bitmaps::rank_table[Rank{Rank::count - 1}]) |
                (bitmaps::rank_table[square.rank()] & ~bitmaps::file_table[File{0}] & ~bitmaps::file_table[File{File::count - 1}])) &
               ~Bitmap{square};
    }
    return Bitmap{};
}

auto blocker_config_count(Bitmap blocker_mask) -> std::uint64_t {
    return 1ULL << blocker_mask.count();
}

auto next_blocker_config(Bitmap blockers, Bitmap mask) -> Bitmap {
    return Bitmap{blockers.bits() - mask.bits()} & mask;
}

auto attack_bitmap(PieceType piece_type, Square square, Bitmap blocker_config) -> Bitmap {
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

auto total_size(const MagicDataSet &data_set) -> std::size_t {
    return std::accumulate(data_set.begin(), data_set.end(), std::size_t{0}, [](std::size_t total, const MagicData &data) { return total + data.max_index + 1; });
}

auto MagicBitboard::init() -> void {
    const auto &data = *m_data_set;
    m_attack_maps.resize(total_size(data));

    std::uint32_t current_offset{0};
    Square square{Square::A1};
    for (int i = 0; i < Square::count; ++i) {
        m_magics[square].offset = current_offset;
        m_magics[square].blocker_mask = blocker_mask(m_piece, square);
        m_magics[square].shift = data[square].shift;
        m_magics[square].magic_number = data[square].magic_number;

        fill_table(m_magics[square], square, current_offset);
        current_offset += data[square].max_index + 1;
        square += 1;
    }
}

auto MagicBitboard::fill_table(const Magics &magics, Square square, std::uint32_t offset) -> void {
    chesscore::Bitmap blockers{};
    do { // NOLINT(cppcoreguidelines-avoid-do-while)
        const auto index = magic_index(blockers, magics.magic_number, magics.shift);
        const auto attack_map = attack_bitmap(m_piece, square, blockers);
        m_attack_maps[offset + index] = attack_map;
        blockers = chesscore::next_blocker_config(blockers, magics.blocker_mask);
    } while (!blockers.empty());
}

} // namespace chesscore
