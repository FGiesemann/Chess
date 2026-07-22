/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#include "magic_bitboard.h"

#include <chesscore/magic.h>

#include <random>

auto fill_table(const TableSpec &spec, const Magics &magics) -> GeneratorResult {
    GeneratorResult result{};
    result.table.reserve(1ULL << (64 - magics.shift));
    const auto blocker_mask = chesscore::blocker_mask(spec.piece, spec.square);
    result.expected_entries = chesscore::blocker_config_count(blocker_mask);

    chesscore::Bitmap blockers{};
    do { // NOLINT(cppcoreguidelines-avoid-do-while)
        const auto index = chesscore::magic_index(blockers, magics.magic_number, magics.shift);
        result.max_index = std::max(result.max_index, index);

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

auto search_magic_number(const TableSpec &spec, const SearchParams &params) -> SearchResult {
    std::mt19937_64 rng{params.rand_seed};
    std::uniform_int_distribution<std::uint64_t> dist{0, std::numeric_limits<std::uint64_t>::max()};
    SearchResult search_result{};

    std::uint64_t max_index{std::numeric_limits<std::uint64_t>::max()};
    for (std::size_t i = 0; i < params.max_tries; ++i) {
        search_result.tries++;
        const auto magic_number = dist(rng);
        const auto magics = Magics{.magic_number = magic_number, .shift = params.shift};
        const auto result = fill_table(spec, magics);
        if (search_result.magic_number == 0) {
            search_result.generator_result = result;
        }
        if (!result.collision_index.has_value()) {
            if (result.max_index < max_index) {
                max_index = result.max_index;
                search_result.magic_number = magic_number;
                search_result.generator_result = result;
            }
        }
    }
    return search_result;
}
