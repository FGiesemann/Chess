/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESS_CORE_IO_BITBOARD_IO_H
#define CHESS_CORE_IO_BITBOARD_IO_H

#include <iosfwd>

#include "chess_core/bitboard.h"

namespace chess_core {

auto operator<<(std::ostream &os, const Bitboard &bitboard) -> std::ostream &;

} // namespace chess_core

#endif
