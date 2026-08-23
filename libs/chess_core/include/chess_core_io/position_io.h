/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_IO_POSITION_IO_H
#define CHESSCORE_IO_POSITION_IO_H

#include <iostream>

#include "chess_core/position.h"
#include "chess_core/position_types.h"

namespace chesscore {

auto operator<<(std::ostream &os, const CastlingRights &rights) -> std::ostream &;
auto operator<<(std::ostream &os, const Position &position) -> std::ostream &;

} // namespace chesscore

#endif
