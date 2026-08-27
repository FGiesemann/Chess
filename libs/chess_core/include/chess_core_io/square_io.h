/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESS_CORE_IO_SQUARE_IO_H
#define CHESS_CORE_IO_SQUARE_IO_H

#include <iosfwd>

#include "chess_core/square.h"

namespace chess_core {

auto operator<<(std::ostream &os, File file) -> std::ostream &;
auto operator<<(std::ostream &os, Rank rank) -> std::ostream &;
auto operator<<(std::ostream &os, Square square) -> std::ostream &;

} // namespace chess_core

#endif
