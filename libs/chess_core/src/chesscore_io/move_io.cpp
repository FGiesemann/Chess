/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore_io/move_io.h"
#include "chesscore_io/piece_io.h"
#include "chesscore_io/square_io.h"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace chesscore {

namespace {

std::string to_uppercase(const std::string &str) {
    std::string result = str;
    for (char &c : result) {
        c = std::toupper(c);
    }
    return result;
}

} // namespace

auto operator<<(std::ostream &os, const Move &move) -> std::ostream & {
    if (move.piece.type != PieceType::Pawn) {
        os << move.piece;
    }
    os << move.from;
    os << (move.captured ? "x" : "-");
    os << move.to;
    if (move.promoted) {
        os << "=" << move.promoted.value();
    }
    if (move.capturing_en_passant) {
        os << " (e.p.)";
    }
    if (move.captured) {
        os << " (x" << move.captured.value() << ")";
    }
    return os;
}

auto write_move_constructor(std::ostream &os, const Move &move) -> std::ostream & {
    std::stringstream sstr;
    os << "Move{";
    sstr << move.to;
    os << ".from = Square::" << to_uppercase(sstr.str()) << ", ";
    sstr.str("");
    sstr << move.to;
    os << ".to = Square::" << to_uppercase(sstr.str()) << ", ";
    os << ".piece = Piece::" << symbolic_name(move.piece);
    if (move.captured) {
        os << ", .captured = Piece::" << symbolic_name(move.captured.value());
    }
    if (move.promoted) {
        os << ", .promoted = Piece::" << symbolic_name(move.promoted.value());
    }
    if (move.capturing_en_passant) {
        os << ", .capturing_en_passant = true";
    }
    os << '}';

    return os;
}

auto operator<<(std::ostream &os, const MoveList &moves) -> std::ostream & {
    std::ranges::for_each(moves, [&](const Move &move) { os << move << '\n'; });
    return os;
}

} // namespace chesscore
