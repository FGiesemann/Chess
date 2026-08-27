/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESS_CORE_IO_MOVE_IO_H
#define CHESS_CORE_IO_MOVE_IO_H

#include <iosfwd>

#include "chess_core/move.h"

namespace chess_core {

auto operator<<(std::ostream &os, const Move &move) -> std::ostream &;
auto write_move_constructor(std::ostream &os, const Move &move) -> std::ostream &;
auto operator<<(std::ostream &os, const MoveList &moves) -> std::ostream &;

} // namespace chess_core

#endif
