/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore_io/piece_io.h"

#include <iostream>

namespace chesscore {

auto operator<<(std::ostream &os, const Piece &piece) -> std::ostream & {
    const auto index = static_cast<std::underlying_type_t<PieceType>>(piece.type) + (piece.color == Color::White ? 0 : piece_type_count);
    return os << "PRNBQKprnbqk"[index];
}

} // namespace chesscore
