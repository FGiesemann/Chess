/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_IO_PIECE_IO_H
#define CHESSCORE_IO_PIECE_IO_H

#include <iosfwd>

#include "chesscore/piece.h"

namespace chesscore {

auto operator<<(std::ostream &os, const Piece &piece) -> std::ostream &;

} // namespace chesscore

#endif
