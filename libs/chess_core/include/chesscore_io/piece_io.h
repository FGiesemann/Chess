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
auto symbolic_name(const Piece &piece) -> std::string;
auto symbolic_type_name(PieceType type) -> std::string;
auto color_name(Color color) -> std::string;
auto operator<<(std::ostream &os, Color color) -> std::ostream &;

} // namespace chesscore

#endif
