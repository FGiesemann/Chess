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

auto symbolic_name(const Piece &piece) -> std::string {
    return color_name(piece.color) + symbolic_type_name(piece.type);
}

auto symbolic_type_name(PieceType type) -> std::string {
    switch (type) {
    case PieceType::Pawn:
        return "Pawn";
    case PieceType::Rook:
        return "Rook";
    case PieceType::Knight:
        return "Knight";
    case PieceType::Bishop:
        return "Bishop";
    case PieceType::Queen:
        return "Queen";
    case PieceType::King:
        return "King";
    }
}

auto color_name(Color color) -> std::string {
    return color == Color::White ? "White" : "Black";
}

} // namespace chesscore
