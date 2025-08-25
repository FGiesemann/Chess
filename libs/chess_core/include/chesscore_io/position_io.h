/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_IO_POSITION_IO_H
#define CHESSCORE_IO_POSITION_IO_H

#include <iostream>

#include "chesscore/position.h"
#include "chesscore/position_types.h"

namespace chesscore {

auto operator<<(std::ostream &os, const CastlingRights &rights) -> std::ostream &;

template<typename BoardT>
auto operator<<(std::ostream &os, const Position<BoardT> &position) -> std::ostream & {
    os << position.board() << '\n';
    os << position.side_to_move() << " to move\n";
    os << "Move counter: " << position.fullmove_number() << " | Halfmove counter " << position.halfmove_clock() << '\n';
    os << "Castling rights: " << position.castling_rights() << " | en passant: ";
    if (position.en_passant_target().has_value()) {
        os << position.en_passant_target().value();
    } else {
        os << "-";
    }
    os << '\n';
    return os;
}

} // namespace chesscore

#endif
