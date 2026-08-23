/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chess_core/bitboard.h"
#include "chess_core/position.h"

using namespace chesscore;

TEST_CASE("Core.Position.UnmakeMove.SinglePawn", "[Position][UnmakeMove]") {
    Position position{FenString::starting_position()};

    Move move{
        .from = Square::E2,
        .to = Square::E4,
        .piece = Piece::WhitePawn,
        .halfmove_clock_before = position.halfmove_clock(),
        .castling_rights_before{position.castling_rights()},
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
    CHECK_FALSE(position.en_passant_target().valid());
}

TEST_CASE("Core.Position.UnmakeMove.Capture", "[Position][UnmakeMove]") {
    Position position{FenString{"8/pp6/1q3n1B/8/1P6/2N2N2/6PP/6R1 b - b3 18 9"}};

    Move m{
        .from = Square::B6,
        .to = Square::G1,
        .piece = Piece::BlackQueen,
        .captured = Piece::WhiteRook,
        .halfmove_clock_before = position.halfmove_clock(),
        .castling_rights_before = position.castling_rights(),
        .en_passant_target_before = position.en_passant_target(),
    };

    CHECK(position.fullmove_number() == 9);
    CHECK(position.halfmove_clock() == 18);
    CHECK(position.en_passant_target() == Square::B3);
    CHECK_FALSE(position.castling_rights()['K']);
    CHECK_FALSE(position.castling_rights()['Q']);
    CHECK_FALSE(position.castling_rights()['k']);
    CHECK_FALSE(position.castling_rights()['q']);
    position.make_move(m);
    CHECK(position.fullmove_number() == 10);
    CHECK(position.halfmove_clock() == 0);
    CHECK_FALSE(position.en_passant_target().valid());

    position.unmake_move(m);
    CHECK(position.fullmove_number() == 9);
    CHECK(position.halfmove_clock() == 18);
    CHECK(position.en_passant_target() == Square::B3);
    CHECK_FALSE(position.castling_rights()['K']);
    CHECK_FALSE(position.castling_rights()['Q']);
    CHECK_FALSE(position.castling_rights()['k']);
    CHECK_FALSE(position.castling_rights()['q']);
}
