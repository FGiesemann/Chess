/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#include "magic_bitboard.h"

#include <chesscore/magic.h>

#include <random>

auto MagicBitboardGenerator::fill_table(const Magics &magics) const -> GeneratorResult {
    GeneratorResult result{};
    const std::size_t table_size = 1ULL << (64 - magics.shift);
    result.table.resize(table_size);
    std::vector<bool> occupied(table_size, false);

    const auto blocker_mask = chesscore::blocker_mask(m_spec.piece, m_spec.square);
    result.expected_entries = chesscore::blocker_config_count(blocker_mask);

    chesscore::Bitmap blockers{};
    do { // NOLINT(cppcoreguidelines-avoid-do-while)
        const auto index = chesscore::magic_index(blockers, magics.magic_number, magics.shift);
        result.max_index = std::max(result.max_index, index);

        const auto attack_map = chesscore::attack_bitmap(m_spec.piece, m_spec.square, blockers);

        if (occupied[index]) {
            if (result.table[index] != attack_map) {
                result.collision_index = index;
                return result;
            }
            result.constructive_collisions++;
        } else {
            occupied[index] = true;
            result.stored_entries++;
            result.table[index] = attack_map;
        }
        blockers = chesscore::next_blocker_config(blockers, blocker_mask);
    } while (!blockers.empty());

    return result;
}

auto MagicBitboardGenerator::search(const SearchParams &params) -> SearchResult {
    std::mt19937_64 rng{m_rand_seed == 0 ? std::random_device{}() : m_rand_seed};
    SearchResult search_result{};
    search_result.generator_result.max_index = std::numeric_limits<std::uint64_t>::max();

    for (std::uint64_t shift : params.shifts) {
        for (std::size_t i = 0; i < params.max_tries; ++i) {
            search_result.tries++;
            const auto magic_number = rng() & rng() & rng();
            const auto magics = Magics{.magic_number = magic_number, .shift = shift};
            const auto result = fill_table(magics);
            const auto found = update_result(search_result, {.magic_number = magic_number, .shift = shift}, result);

            if (found && m_early_exit) {
                return search_result;
            }
        }
    }
    return search_result;
}

auto MagicBitboardGenerator::update_result(SearchResult &search_result, const Magics &magics,
                                           const GeneratorResult &result) -> bool {
    if (result.successful()) {
        if (result.max_index < search_result.generator_result.max_index) {
            search_result.magics = magics;
            search_result.generator_result = result;
            if (m_progress_callback) {
                m_progress_callback(search_result);
            }
            return true;
        }
        if (m_report_all_magics && m_progress_callback) {
            SearchResult r{search_result};
            r.magics = magics;
            r.generator_result = result;
            m_progress_callback(r);
            return false;
        }
    }
    return false;
}

auto make_shift_range(std::uint64_t start, std::uint64_t end) -> Shifts {
    std::uint64_t inc = start < end ? 1 : -1;
    Shifts shifts{};
    for (std::uint64_t i = start; i != end; i += inc) {
        shifts.push_back(i);
    }
    if (start != end) {
        shifts.push_back(end);
    }
    return shifts;
}
