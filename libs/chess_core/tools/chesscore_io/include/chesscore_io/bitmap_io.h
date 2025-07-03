/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_IO_BITMAP_IO_H
#define CHESSCORE_IO_BITMAP_IO_H

#include <iosfwd>

#include "chesscore/bitmap.h"

namespace chesscore {

auto operator<<(std::ostream &os, const Bitmap &bitmap) -> std::ostream &;
auto as_grouped_hex(const Bitmap &bitmap) -> std::string;
auto as_ull_hex(const Bitmap &bitmap) -> std::string;
auto as_grouped_bits(const Bitmap &bitmap) -> std::string;

} // namespace chesscore

#endif
