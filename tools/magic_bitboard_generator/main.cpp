/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#include "magic_bitboard.h"

#include <iostream>

auto print_search_status(const SearchResult &result) -> void {
    const auto stats = result.generator_result;
    std::cout << "Statistics:\n";
    std::cout << std::format("  Stored entries: {} / {}", stats.stored_entries, stats.expected_entries);
    if (stats.expected_entries != 0) {
        std::cout << std::format(" = {:.2f} %", static_cast<double>(stats.stored_entries) * 100.0 /
                                                    static_cast<double>(stats.expected_entries));
    }
    std::cout << std::format("\n  Max index: {}\n", stats.max_index);
    std::cout << std::format("  Constructive collisions: {}\n", stats.constructive_collisions);
    if (stats.collision_index.has_value()) {
        std::cout << std::format("  Collision at index: {}\n", stats.collision_index.value());
    } else {
        std::cout << std::format("  Tries: {}\n", result.tries);
        std::cout << std::format("Magic number: 0x{:016x} | Index bits: {}\n", result.magic_number, 64 - result.shift);
    }
}

auto main(int argc, const char *argv[]) -> int {
    std::cout << "Magic Bitboard Generator\n";

    const auto result = search_magic_number({.piece = chesscore::PieceType::Rook, .square = chesscore::Square::E4},
                                            {.rand_seed = 0,
                                             .max_tries = 10'000'000,
                                             .shifts = make_shift_range(64 - 12, 64 - 10),
                                             .process_report_callback = print_search_status,
                                             .report_all_magics = true});

    print_search_status(result);
}
