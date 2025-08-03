/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_IO_MOVE_IO_H
#define CHESSCORE_IO_MOVE_IO_H

#include <iosfwd>

#include "chesscore/move.h"

namespace chesscore {

auto operator<<(std::ostream &os, const Move &move) -> std::ostream &;
auto write_move_constructor(std::ostream &os, const Move &move) -> std::ostream &;
auto operator<<(std::ostream &os, const MoveList &moves) -> std::ostream &;

} // namespace chesscore

#endif
