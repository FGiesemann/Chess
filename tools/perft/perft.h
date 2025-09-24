/* ************************************************************************** *
 * Chess Tools - Perft                                                        *
 * Debugging and Tools for Chess                                              *
 * Performance Test (perft-Tool)                                              *
 * ************************************************************************** */

#ifndef CHESS_TOOLS_PERFT_PERFT_H
#define CHESS_TOOLS_PERFT_PERFT_H

#include <chesscore/bitboard.h>
#include <chesscore/position.h>

#include <chrono>
#include <vector>

namespace chesstools::perft {

struct PerftResult {
    std::uint64_t node_count{0};
    std::chrono::microseconds duration{};
};

struct DivideEntry {
    chesscore::Move move;
    std::uint64_t node_count;
};

using DivideTable = std::vector<DivideEntry>;

struct DivideResult {
    DivideTable table{};
    std::chrono::microseconds duration{};
};

auto perft(chesscore::Position &pos, int depth) -> PerftResult;
auto perft_count(chesscore::Position &pos, int depth) -> std::uint64_t;
auto divide(chesscore::Position &pos, int depth) -> DivideResult;

} // namespace chesstools::perft

#endif
