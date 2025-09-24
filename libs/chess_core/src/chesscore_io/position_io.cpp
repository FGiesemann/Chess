/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore_io/position_io.h"
#include "chesscore_io/bitboard_io.h"
#include "chesscore_io/piece_io.h"
#include "chesscore_io/square_io.h"

namespace chesscore {

auto operator<<(std::ostream &os, const CastlingRights &rights) -> std::ostream & {
    bool found_one{false};
    if (rights['K']) {
        os << 'K';
        found_one = true;
    }
    if (rights['Q']) {
        os << 'Q';
        found_one = true;
    }
    if (rights['k']) {
        os << 'k';
        found_one = true;
    }
    if (rights['q']) {
        os << 'q';
        found_one = true;
    }
    if (!found_one) {
        os << '-';
    }
    return os;
}

auto operator<<(std::ostream &os, const Position &position) -> std::ostream & {
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
