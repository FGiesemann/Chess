/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/perft.h"

namespace chesscore {

auto perft(Position &pos, int depth) -> std::uint64_t {
    PerftCounter<PerftMode::Verify> counter;
    perft(pos, depth, counter);
    return counter.leaf_nodes;
};

} // namespace chesscore
