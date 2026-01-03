/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_PERFT_H
#define CHESSCORE_PERFT_H

#include "chesscore/position.h"

namespace chesscore {

auto perft(Position &pos, int depth) -> std::uint64_t;

/**
 * \brief Modus for generic perft function
 *
 * The generic perft function can be used for the classic correctness checks,
 * but also count all process nodes for performance analysis.
 */
enum class PerftMode {
    Verify,   ///< Standard correctness check
    Benchmark ///< Performance analysis with total node count
};

template<PerftMode Mode>
struct PerftCounter {
    std::uint64_t leaf_nodes{};
    std::uint64_t total_nodes{};

    /**
     * \brief Count internal nodes.
     *
     * Internal nodes are only counted with PerftMode::Benchmark. In
     * PerftMode::Verify, this code is completely removed.
     */
    inline void count_node() {
        if constexpr (Mode == PerftMode::Benchmark) {
            total_nodes++;
        }
    }

    /**
     * \brief Count leaf nodes.
     *
     * Leaf nodes are counted in verification and performance analysis modes.
     */
    inline void count_leaf_node() { leaf_nodes++; }
};

template<PerftMode Mode>
auto perft(Position &position, int depth, PerftCounter<Mode> &counter) -> void {
    counter.count_node();

    if (depth == 0) {
        counter.count_leaf_node();
        return;
    }

    auto moves = position.all_legal_moves();
    for (const auto &move : moves) {
        position.make_move(move);
        perft<Mode>(position, depth - 1, counter);
        position.unmake_move(move);
    }
}

} // namespace chesscore

#endif
