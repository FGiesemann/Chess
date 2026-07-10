/* ************************************************************************** *
 * Chess Tools - Perft                                                        *
 * Debugging and Tools for Chess                                              *
 * Performance Test (perft-Tool)                                              *
 * ************************************************************************** */

#include "perft.h"

namespace chesstools::perft {

namespace {

auto create_perft_result(std::uint64_t count, const std::chrono::high_resolution_clock::time_point &start_time) -> PerftResult {
    const auto end_time = std::chrono::high_resolution_clock::now();
    return PerftResult{.node_count = count, .duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time)};
}

} // namespace

auto perft(chesscore::Position &pos, int depth) -> PerftResult {
    const auto start_time = std::chrono::high_resolution_clock::now();
    if (depth == 0) {
        return create_perft_result(1, start_time);
    }

    const auto moves = pos.all_legal_moves();
    if (depth == 1) {
        return create_perft_result(moves.size(), start_time);
    }

    std::uint64_t count{0};
    for (const auto &move : moves) {
        pos.make_move(move);
        count += perft_count(pos, depth - 1);
        pos.unmake_move(move);
    }
    return create_perft_result(count, start_time);
}

auto perft_count(chesscore::Position &pos, int depth) -> std::uint64_t {
    if (depth == 0) {
        return 1;
    }

    const auto moves = pos.all_legal_moves();
    if (depth == 1) {
        return moves.size();
    }

    std::uint64_t count{0};
    for (const auto &move : moves) {
        pos.make_move(move);
        count += perft(pos, depth - 1).node_count;
        pos.unmake_move(move);
    }
    return count;
}

auto divide(chesscore::Position &pos, int depth) -> DivideResult {
    if (depth == 0) {
        return {};
    }

    DivideResult result{};
    const auto start_time = std::chrono::high_resolution_clock::now();
    const auto moves = pos.all_legal_moves();
    for (const auto &move : moves) {
        pos.make_move(move);
        const auto count = perft_count(pos, depth - 1);
        pos.unmake_move(move);
        result.table.emplace_back(move, count);
    }
    const auto end_time = std::chrono::high_resolution_clock::now();
    result.duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    return result;
}

} // namespace chesstools::perft
