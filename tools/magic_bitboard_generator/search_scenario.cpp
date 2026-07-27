/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#include "search_scenario.h"

#include "database.h"
#include "magic_bitboard.h"

#include <filesystem>
#include <iostream>

namespace magic_bitboard_generator::search_scenario {

auto write_database(const Database &db, const std::filesystem::path &database_path) -> void {
    DatabaseWriter{database_path}.write(db);
}

auto print_search_status(Database &database, const Args &args, const std::string &database_path, const TableSpec &table,
                         const SearchResult &result) -> void {
    const auto stats = result.generator_result;
    if (stats.successful()) {
        Magics magics = result.magics;
        TableStats stats{.blocker_configs = result.generator_result.expected_entries,
                         .max_index = result.generator_result.max_index,
                         .constructive_collisions = result.generator_result.constructive_collisions};
        bool better_result{false};
        if (database.record(table.piece, table.square).update_magics(magics, stats)) {
            write_database(database, database_path);
            better_result = true;
        }
        std::cout << std::format("Magic number: 0x{:016x} | Index bits: {:2d} | Max index: {:5d}{} {:5d} | Constr. "
                                 "coll.: {:3d} | Tries: {}\n",
                                 result.magics.magic_number, 64 - result.magics.shift, stats.max_index,
                                 better_result ? '*' : ' ', result.generator_result.expected_entries,
                                 result.generator_result.constructive_collisions, result.tries);

    } else {
        std::cout << std::format("  Stored entries: {} / {}", stats.stored_entries, stats.expected_entries);
        if (stats.expected_entries != 0) {
            std::cout << std::format(" = {:.2f} %", static_cast<double>(stats.stored_entries) * 100.0 /
                                                        static_cast<double>(stats.expected_entries));
        }
        std::cout << std::format("\n  Max index: {}\n", stats.max_index);
        std::cout << std::format("  Constructive collisions: {}\n", stats.constructive_collisions);
        std::cout << std::format("  Collision at index: {}\n", stats.collision_index.value());
    }
}

auto search_magic(const Args &args) -> void {
    const std::string database_path = args.database.empty() ? "magic_bitboard_database.txt" : args.database;

    Database database;
    if (std::filesystem::exists(database_path)) {
        DatabaseReader reader{database_path};
        database = reader.read();
    }

    for (const auto piece : args.piece_types) {
        for (const auto &square : args.squares) {
            TableSpec table_spec{.piece = piece, .square = square};
            std::cout << "Magic Bitboard Generator for " << chesscore::to_string(table_spec.piece) << " at "
                      << to_string(table_spec.square) << '\n';

            MagicBitboardGenerator generator{table_spec};
            generator.set_progress_callback([&](const SearchResult &result) -> void {
                print_search_status(database, args, database_path, table_spec, result);
            });

            const auto result = generator.search({.max_tries = args.iterations, .shifts = args.shifts});
        }
    }
    write_database(database, database_path);
}

} // namespace magic_bitboard_generator::search_scenario
