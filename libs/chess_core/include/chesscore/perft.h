/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_PERFT_H
#define CHESSCORE_PERFT_H

#include "chesscore/position.h"

namespace chesscore {

template<typename BoardT>
auto perft(Position<BoardT> &pos, int depth) -> std::uint64_t {
    if (depth == 0) {
        return 1;
    }

    std::uint64_t count{0};
    const auto moves = pos.all_legal_moves();
    for (const auto &move : moves) {
        pos.make_move(move);
        count += perft(pos, depth - 1);
        pos.unmake_move(move);
    }
    return count;
};

} // namespace chesscore

#endif
