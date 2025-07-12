/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/bitboard.h"
#include "chesscore/fen.h"
#include "chesscore/move.h"
#include "chesscore/position.h"

using namespace chesscore;

TEST_CASE("Bitboard.Bitboard.IsAttacked.PawnAttacks", "[Bitboard][Attacks]") {
    Bitboard board{FenString{"r1N5/1Ppp4/5p2/2N3b1/pP3P1R/4rPP1/1p1P4/b7 w - - 0 1"}};

    CHECK(board.pawn_attacks(Square::A8, Color::White));
    CHECK(board.pawn_attacks(Square::C8, Color::White));
    CHECK(board.pawn_attacks(Square::B5, Color::White));
    CHECK(board.pawn_attacks(Square::C5, Color::White));
    CHECK(board.pawn_attacks(Square::D3, Color::White));
    CHECK(board.pawn_attacks(Square::E3, Color::White));
    CHECK(board.pawn_attacks(Square::F5, Color::White));
    CHECK(board.pawn_attacks(Square::G5, Color::White));
    CHECK(board.pawn_attacks(Square::F4, Color::White));
    CHECK(board.pawn_attacks(Square::G4, Color::White));
    CHECK(board.pawn_attacks(Square::F4, Color::White));
    CHECK(board.pawn_attacks(Square::H4, Color::White));

    CHECK(board.pawn_attacks(Square::B3, Color::Black));
    CHECK(board.pawn_attacks(Square::A1, Color::Black));
    CHECK(board.pawn_attacks(Square::C1, Color::Black));
    CHECK(board.pawn_attacks(Square::B6, Color::Black));
    CHECK(board.pawn_attacks(Square::D6, Color::Black));
    CHECK(board.pawn_attacks(Square::C6, Color::Black));
    CHECK(board.pawn_attacks(Square::E6, Color::Black));
    CHECK(board.pawn_attacks(Square::E5, Color::Black));
    CHECK(board.pawn_attacks(Square::G5, Color::Black));

    CHECK_FALSE(board.pawn_attacks(Square::C2, Color::White));
    CHECK_FALSE(board.pawn_attacks(Square::C2, Color::Black));
    CHECK_FALSE(board.pawn_attacks(Square::B8, Color::White));
    CHECK_FALSE(board.pawn_attacks(Square::B8, Color::Black));
    CHECK_FALSE(board.pawn_attacks(Square::B3, Color::White));
    CHECK_FALSE(board.pawn_attacks(Square::A5, Color::Black));
}

TEST_CASE("Bitboard.Bitboard.IsAttacked.KnightAttacks", "[Bitboard][Attacks]") {}

TEST_CASE("Bitboard.Bitboard.IsAttacked.SlidingPieceAttacks", "[Bitboard][Attacks]") {}

TEST_CASE("Bitboard.Bitboard.IsAttacked.ByWhite", "[Bitboard][Attacks]") {
    Bitboard board{FenString{"5n2/2Q5/6k1/1B1n3P/4r3/R7/5N2/8 w - - 0 1"}};

    CHECK(board.is_attacked(Square::A1, Color::White)); // Rxa1
    CHECK(board.is_attacked(Square::C3, Color::White)); // Rxc3 Qxc3
    CHECK(board.is_attacked(Square::D3, Color::White)); // Rxd3 Bxd3 Nxd3
    CHECK(board.is_attacked(Square::G4, Color::White)); // Nxg4
    CHECK(board.is_attacked(Square::E2, Color::White)); // Bxe2
    CHECK(board.is_attacked(Square::A6, Color::White)); // Rxa6 Bxa6
    CHECK(board.is_attacked(Square::E4, Color::White)); // Nxe4
    CHECK(board.is_attacked(Square::G6, Color::White)); // xg6
    CHECK(board.is_attacked(Square::D7, Color::White)); // Bxd7 Qxd7
    CHECK(board.is_attacked(Square::H7, Color::White)); // Qxh7

    // exhaustive check:
    CHECK_FALSE(board.is_attacked(Square::A3, Color::White));
    CHECK_FALSE(board.is_attacked(Square::B1, Color::White));
    CHECK_FALSE(board.is_attacked(Square::B2, Color::White));
    CHECK_FALSE(board.is_attacked(Square::B4, Color::White));
    CHECK_FALSE(board.is_attacked(Square::B5, Color::White));
    CHECK_FALSE(board.is_attacked(Square::C7, Color::White));
    CHECK_FALSE(board.is_attacked(Square::D2, Color::White));
    CHECK_FALSE(board.is_attacked(Square::D4, Color::White));
    CHECK_FALSE(board.is_attacked(Square::E1, Color::White));
    CHECK_FALSE(board.is_attacked(Square::E6, Color::White));
    CHECK_FALSE(board.is_attacked(Square::F5, Color::White));
    CHECK_FALSE(board.is_attacked(Square::F6, Color::White));
    CHECK_FALSE(board.is_attacked(Square::F8, Color::White));
    CHECK_FALSE(board.is_attacked(Square::G1, Color::White));
    CHECK_FALSE(board.is_attacked(Square::G2, Color::White));
    CHECK_FALSE(board.is_attacked(Square::G5, Color::White));
    CHECK_FALSE(board.is_attacked(Square::G8, Color::White));
    CHECK_FALSE(board.is_attacked(Square::H4, Color::White));
    CHECK_FALSE(board.is_attacked(Square::H5, Color::White));
    CHECK_FALSE(board.is_attacked(Square::H6, Color::White));
    CHECK_FALSE(board.is_attacked(Square::H8, Color::White));
}

TEST_CASE("Bitboard.Bitboard.IsAttacked.ByBlack", "[Bitboard][Attacks]") {
    Bitboard board{FenString{"q4n2/2Q5/6k1/1B1n3P/4r3/R7/5N2/2b5 b - - 0 1"}};

    CHECK(board.is_attacked(Square::A3, Color::Black)); // Bxa3 Qxa3
    CHECK(board.is_attacked(Square::A4, Color::Black)); // Qxa4 Rxa4
    CHECK(board.is_attacked(Square::B2, Color::Black)); // Bxa2
    CHECK(board.is_attacked(Square::E3, Color::Black)); // Bxe3 Nxe3
    CHECK(board.is_attacked(Square::B8, Color::Black)); // Qxb8
    CHECK(board.is_attacked(Square::D7, Color::Black)); // Nxd7
    CHECK(board.is_attacked(Square::G6, Color::Black)); // Nxg6
    CHECK(board.is_attacked(Square::E6, Color::Black)); // Nxe6 Rxe6
    CHECK(board.is_attacked(Square::G4, Color::Black)); // Rxg4
    CHECK(board.is_attacked(Square::G7, Color::Black)); // Kxg7
    CHECK(board.is_attacked(Square::H7, Color::Black)); // Kxh7 Nxh7
    CHECK(board.is_attacked(Square::H5, Color::Black)); // Kxh5
    CHECK(board.is_attacked(Square::F6, Color::Black)); // Nxf6 Kxf6
    CHECK(board.is_attacked(Square::C7, Color::Black)); // Nxc7

    // exhaustive check:
    CHECK_FALSE(board.is_attacked(Square::A1, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::A2, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::A8, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::B1, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::B3, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::B5, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::C1, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::C2, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::C5, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::D1, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::D3, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::D5, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::D6, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::E4, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::F1, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::F2, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::F3, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::G1, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::G2, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::G3, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::G8, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::H1, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::H2, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::H3, Color::Black));
    CHECK_FALSE(board.is_attacked(Square::H8, Color::Black));
}
