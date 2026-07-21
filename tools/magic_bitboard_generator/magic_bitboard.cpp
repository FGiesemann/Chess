/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#include "magic_bitboard.h"

#include <chesscore/magic.h>

auto fill_table(const TableSpec &spec, const Magics &magics) -> GeneratorResult {
    GeneratorResult result{};
    const auto blocker_mask = chesscore::blocker_mask(spec.piece, spec.square);
    result.expected_entries = chesscore::blocker_config_count(blocker_mask);

    chesscore::Bitmap blockers{};
    do {
        const auto index = chesscore::magic_index(blockers, magics.magic_number, magics.shift);
        if (index >= result.table.size()) {
            result.table.resize(index + 1);
        }
        const auto attack_map = chesscore::attack_bitmap(spec.piece, spec.square, blockers);
        const auto stored_map = result.table[index];
        if (!stored_map.empty()) {
            if (stored_map != attack_map) {
                result.collision_index = index;
                return result;
            }
            result.constructive_collisions++;
        } else {
            result.stored_entries++;
            result.table[index] = attack_map;
        }
        blockers = chesscore::next_blocker_config(blockers, blocker_mask);
    } while (!blockers.empty());

    return result;
}
