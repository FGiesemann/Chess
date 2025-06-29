/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/bitboard.h"
#include "chesscore/position.h"

using namespace chesscore;

TEST_CASE("Position.UnmakeMove.SinglePawn", "[Position][UnmakeMove]") {
    Position<Bitboard> position{FenString::starting_position()};

    Move move{
        .from = Square::E2,
        .to = Square::E4,
        .piece = Piece::WhitePawn,
        .castling_rights_before{position.castling_rights()},
        .halfmove_clock_before = position.halfmove_clock(),
    };

    position.make_move(move);
    position.unmake_move(move);

    CHECK(position.halfmove_clock() == 0);
    CHECK(position.fullmove_number() == 1);
    CHECK(position.side_to_move() == Color::White);
    CHECK(position.castling_rights()['K']);
    CHECK(position.castling_rights()['Q']);
    CHECK(position.castling_rights()['k']);
    CHECK(position.castling_rights()['q']);
    CHECK_FALSE(position.en_passant_target().has_value());
}
