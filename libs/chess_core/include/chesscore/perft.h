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

} // namespace chesscore

#endif
