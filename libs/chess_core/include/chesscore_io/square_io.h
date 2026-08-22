/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_IO_SQUARE_IO_H
#define CHESSCORE_IO_SQUARE_IO_H

#include <iosfwd>

#include "chesscore/square.h"

namespace chesscore {

auto operator<<(std::ostream &os, File file) -> std::ostream &;
auto operator<<(std::ostream &os, Rank rank) -> std::ostream &;
auto operator<<(std::ostream &os, Square square) -> std::ostream &;

} // namespace chesscore

#endif
