/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_IO_BITBOARD_IO_H
#define CHESSCORE_IO_BITBOARD_IO_H

#include <iosfwd>

#include "chesscore/bitboard.h"

namespace chesscore {

auto operator<<(std::ostream &os, const Bitboard &bitmap) -> std::ostream &;

} // namespace chesscore

#endif
