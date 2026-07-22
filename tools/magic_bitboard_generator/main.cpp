/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#include "magic_bitboard.h"

#include <iostream>

auto print_search_status(const SearchResult &result) -> void {
    const auto stats = result.generator_result;
    if (stats.successful()) {
        std::cout << std::format(
            "Magic number: 0x{:016x} | Index bits: {} | Max index: {} | Constr. coll.: {} | Tries: {}\n",
            result.magic_number, 64 - result.shift, stats.max_index, result.generator_result.constructive_collisions,
            result.tries);
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
    const TableSpec table_spec{.piece = chesscore::PieceType::Rook, .square = chesscore::Square::A1};

    std::cout << "Magic Bitboard Generator for " << chesscore::to_string(table_spec.piece) << " at "
              << to_string(table_spec.square) << '\n';

    MagicBitboardGenerator generator{table_spec};
    generator.set_progress_callback(print_search_status);

    const auto result = generator.search({.max_tries = 10'000'000, .shifts = make_shift_range(64 - 14, 64 - 10)});

    print_search_status(result);
}
