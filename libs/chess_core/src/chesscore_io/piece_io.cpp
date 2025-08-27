/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore_io/piece_io.h"

#include <iostream>

namespace chesscore {

auto operator<<(std::ostream &os, PieceType type) -> std::ostream & {
    switch (type) {
    case PieceType::Pawn:
        return os << "P";
    case PieceType::Rook:
        return os << "R";
    case PieceType::Knight:
        return os << "N";
    case PieceType::Bishop:
        return os << "B";
    case PieceType::Queen:
        return os << "Q";
    case PieceType::King:
        return os << "K";
    default:
        return os << "<invalid piece type>";
    }
}

auto operator<<(std::ostream &os, const Piece &piece) -> std::ostream & {
    return os << piece.piece_char();
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
    default:
        return "<invalid piece>";
    }
}

auto color_name(Color color) -> std::string {
    return color == Color::White ? "White" : "Black";
}

auto operator<<(std::ostream &os, Color color) -> std::ostream & {
    return os << color_name(color);
}

} // namespace chesscore
