/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#include "chesscore/move.h"

namespace chesscore {

auto is_moving_same_piece(const Move &move1, const Move &move2) -> bool {
    return move1.from == move2.from && move1.to == move2.to && move1.piece == move2.piece && move1.captured == move2.captured &&
           move1.capturing_en_passant == move2.capturing_en_passant;
}

auto BasicMoveCompare::operator()(const Move &move1, const Move &move2) -> bool {
    return is_moving_same_piece(move1, move2);
}

} // namespace chesscore
