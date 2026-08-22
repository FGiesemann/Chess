/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#include "check_scenario.h"
#include "database.h"
#include "magic_bitboard.h"

#include <iostream>

namespace magic_bitboard_generator::check_scenario {

auto check_magics(const Args &args) -> void {
    TableSpec table{.piece = args.piece_types[0], .square = args.squares[0]};
    MagicBitboardGenerator generator(table);
    const Magics magics{.magic_number = args.magic_number, .shift = args.shifts[0]};
    const auto result = generator.fill_table(magics);

    if (result.successful()) {
        TableStats stats{.blocker_configs = result.expected_entries,
                         .max_index = result.max_index,
                         .constructive_collisions = result.constructive_collisions};
        bool better_result{false};
        if (!args.database.empty() && std::filesystem::exists(args.database)) {
            Database database = DatabaseReader{args.database}.read();
            if (database.record(table.piece, table.square).update_magics(magics, stats)) {
                DatabaseWriter{args.database}.write(database);
                better_result = true;
            }
        }
        std::cout << std::format("Magic number: 0x{:016x} | Index bits: {:2d} | Max index: {:5d}{} {:5d} | Constr. "
                                 "coll.: {:3d}\n",
                                 magics.magic_number, 64 - magics.shift, stats.max_index, better_result ? '*' : ' ',
                                 result.expected_entries, result.constructive_collisions);
    } else {
        std::cout << "Collisions:\n";
        std::cout << std::format("  Stored entries: {} / {}", result.stored_entries, result.expected_entries);
        if (result.expected_entries != 0) {
            std::cout << std::format(" = {:.2f} %", static_cast<double>(result.stored_entries) * 100.0 /
                                                        static_cast<double>(result.expected_entries));
        }
        std::cout << std::format("\n  Max index: {}\n", result.max_index);
        std::cout << std::format("  Constructive collisions: {}\n", result.constructive_collisions);
        std::cout << std::format("  Collision at index: {}\n", result.collision_index.value());
    }
}

} // namespace magic_bitboard_generator::check_scenario
