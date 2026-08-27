/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESS_CORE_IO_BITMAP_IO_H
#define CHESS_CORE_IO_BITMAP_IO_H

#include <iosfwd>

#include "chess_core/bitmap.h"

namespace chess_core {

auto operator<<(std::ostream &os, Bitmap bitmap) -> std::ostream &;
auto as_grouped_hex(Bitmap bitmap) -> std::string;
auto as_ull_hex(Bitmap bitmap) -> std::string;
auto as_grouped_bits(Bitmap bitmap) -> std::string;

} // namespace chess_core

#endif
