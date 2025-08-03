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

auto operator<<(std::ostream &os, const File &file) -> std::ostream &;
auto operator<<(std::ostream &os, const Rank &rank) -> std::ostream &;
auto operator<<(std::ostream &, const Square &square) -> std::ostream &;

} // namespace chesscore

#endif
