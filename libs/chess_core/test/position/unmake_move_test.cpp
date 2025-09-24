/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/bitboard.h"
#include "chesscore/position.h"

using namespace chesscore;

TEST_CASE("Position.UnmakeMove.SinglePawn", "[Position][UnmakeMove]") {
    Position position{FenString::starting_position()};

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

TEST_CASE("Position.UnmakeMove.Capture", "[Position][UnmakeMove]") {
    Position position{FenString{"8/pp6/1q3n1B/8/1P6/2N2N2/6PP/6R1 b - b3 18 9"}};

    Move m{
        .from = Square::B6,
        .to = Square::G1,
        .piece = Piece::BlackQueen,
        .captured = Piece::WhiteRook,
        .castling_rights_before = position.castling_rights(),
        .halfmove_clock_before = position.halfmove_clock(),
        .en_passant_target_before = position.en_passant_target(),
    };

    CHECK(position.fullmove_number() == 9);
    CHECK(position.halfmove_clock() == 18);
    CHECK(position.en_passant_target().value_or(Square::A1) == Square::B3);
    CHECK_FALSE(position.castling_rights()['K']);
    CHECK_FALSE(position.castling_rights()['Q']);
    CHECK_FALSE(position.castling_rights()['k']);
    CHECK_FALSE(position.castling_rights()['q']);
    position.make_move(m);
    CHECK(position.fullmove_number() == 10);
    CHECK(position.halfmove_clock() == 0);
    CHECK_FALSE(position.en_passant_target().has_value());

    position.unmake_move(m);
    CHECK(position.fullmove_number() == 9);
    CHECK(position.halfmove_clock() == 18);
    CHECK(position.en_passant_target().value_or(Square::A1) == Square::B3);
    CHECK_FALSE(position.castling_rights()['K']);
    CHECK_FALSE(position.castling_rights()['Q']);
    CHECK_FALSE(position.castling_rights()['k']);
    CHECK_FALSE(position.castling_rights()['q']);
}
