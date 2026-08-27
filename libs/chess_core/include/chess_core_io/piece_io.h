/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESS_CORE_IO_PIECE_IO_H
#define CHESS_CORE_IO_PIECE_IO_H

#include <iosfwd>

#include "chess_core/piece.h"

namespace chess_core {

auto operator<<(std::ostream &os, PieceType type) -> std::ostream &;
auto operator<<(std::ostream &os, Piece piece) -> std::ostream &;
auto symbolic_name(Piece piece) -> std::string;
auto symbolic_type_name(PieceType type) -> std::string;
auto color_name(Color color) -> std::string;
auto operator<<(std::ostream &os, Color color) -> std::ostream &;

} // namespace chess_core

#endif
