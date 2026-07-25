/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#include "arg_parse.h"
#include "database.h"
#include "magic_bitboard.h"

#include <filesystem>
#include <iostream>

using namespace magic_bitboard_generator;

static const auto database_path = std::filesystem::path{"magic_bitboard_database.txt"};

auto write_database(const Database &db) -> void {
    DatabaseWriter{database_path}.write(db);
}

auto print_search_status(Database &database, const TableSpec &table, const SearchResult &result) -> void {
    const auto stats = result.generator_result;
    if (stats.successful()) {
        std::cout << std::format(
            "Magic number: 0x{:016x} | Index bits: {} | Max index: {} | Constr. coll.: {} | Tries: {}\n",
            result.magics.magic_number, 64 - result.magics.shift, stats.max_index,
            result.generator_result.constructive_collisions, result.tries);

        Magics magics = result.magics;
        TableStats stats{.max_index = result.generator_result.max_index,
                         .constructive_collisions = result.generator_result.constructive_collisions};
        if (database.record(table.piece, table.square).update_magics(magics, stats)) {
            write_database(database);
        }
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

auto main(int argc, const char *argv[]) -> int {
    const auto arguments = convert_args({argv, static_cast<std::size_t>(argc)});
    const auto args = parse_arguments(arguments);

    std::cout << "Scenario: " << to_string(args.scenario) << '\n';
    std::cout << "Piece types:\n";
    for (const auto &piece : args.piece_types) {
        std::cout << " " << to_string(piece);
    }
    std::cout << '\n';

    std::cout << "Squares:\n";
    for (const auto &square : args.squares) {
        std::cout << " " << to_string(square);
    }
    std::cout << '\n';

    std::cout << "Magic number: 0x" << std::hex << args.magic_number << std::dec << '\n';
    std::cout << "Shifts:\n";
    for (const auto &shift : args.shifts) {
        std::cout << " " << shift;
    }
    std::cout << '\n';

    std::cout << "Database: " << args.database << '\n';

    // Database database;
    // if (std::filesystem::exists(database_path)) {
    //     DatabaseReader reader{database_path};
    //     database = reader.read();
    // }

    // TableSpec table_spec{.piece = chesscore::PieceType::Rook, .square = chesscore::Square::E4};
    // // for (int i = 0; i < chesscore::Square::count; ++i) {
    // std::cout << "Magic Bitboard Generator for " << chesscore::to_string(table_spec.piece) << " at "
    //           << to_string(table_spec.square) << '\n';

    // MagicBitboardGenerator generator{table_spec};
    // generator.set_progress_callback(
    //     [&](const SearchResult &result) -> void { print_search_status(database, table_spec, result); });

    // const auto result = generator.search({.max_tries = 1'000'000'000, .shifts = make_shift_range(64 - 11, 64 - 8)});

    // print_search_status(database, table_spec, result);
    // table_spec.square += 1;
    // // }
    // write_database(database);
}
