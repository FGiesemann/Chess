/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/perft.h"

namespace chesscore {

auto perft(Position &pos, int depth) -> std::uint64_t {
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
        count += perft(pos, depth - 1);
        pos.unmake_move(move);
    }
    return count;
};

}
