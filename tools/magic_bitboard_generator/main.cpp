/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#include "magic_bitboard.h"

#include <iostream>

auto main(int argc, const char *argv[]) -> int {
    std::cout << "Magic Bitboard Generator\n";

    const auto stats = fill_table({.piece = chesscore::PieceType::Rook, .square = chesscore::Square::E4},
                                  {.magic_number = 0x0e0702e07e7e7e7eULL, .shift = 64 - 12});

    std::cout << "Statistics:\n";
    std::cout << "  Stored entries: " << stats.stored_entries << " / " << stats.expected_entries << " = "
              << static_cast<double>(stats.stored_entries) * 100.0 / static_cast<double>(stats.expected_entries)
              << " %\n";
    std::cout << "  Max index: " << stats.max_index << '\n';
    std::cout << "  Constructive collisions: " << stats.constructive_collisions << '\n';
    if (stats.collision_index.has_value()) {
        std::cout << "  Collision at index: " << stats.collision_index.value() << '\n';
    }
}
