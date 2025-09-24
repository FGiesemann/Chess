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

TEST_CASE("Position.Bitboard.MoveGeneration (1)", "[Position][MoveGeneration]") {
    Position position{FenString{"2k2r2/3bp1q1/1bN5/8/8/4P1NQ/8/r2R2K1 w - - 0 1"}};
    MoveList moves = position.all_legal_moves();
    CHECK(moves.size() == 29);

    CHECK(move_list_contains(moves, Move{Square::C6, Square::B8, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves, Move{Square::C6, Square::D8, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves, Move{Square::C6, Square::A7, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves, Move{Square::C6, Square::E7, Piece::WhiteKnight, Piece::BlackPawn}));
    CHECK(move_list_contains(moves, Move{Square::C6, Square::E5, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves, Move{Square::C6, Square::A5, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves, Move{Square::C6, Square::B4, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves, Move{Square::C6, Square::D4, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves, Move{Square::D1, Square::A1, Piece::WhiteRook, Piece::BlackRook}));
    CHECK(move_list_contains(moves, Move{Square::D1, Square::B1, Piece::WhiteRook}));
    CHECK(move_list_contains(moves, Move{Square::D1, Square::C1, Piece::WhiteRook}));
    CHECK(move_list_contains(moves, Move{Square::D1, Square::E1, Piece::WhiteRook}));
    CHECK(move_list_contains(moves, Move{Square::D1, Square::F1, Piece::WhiteRook}));
    CHECK(move_list_contains(moves, Move{Square::G1, Square::G2, Piece::WhiteKing}));
    CHECK(move_list_contains(moves, Move{Square::G1, Square::H2, Piece::WhiteKing}));
    CHECK(move_list_contains(moves, Move{Square::G1, Square::H1, Piece::WhiteKing}));
    CHECK(move_list_contains(moves, Move{Square::H3, Square::D7, Piece::WhiteQueen, Piece::BlackBishop}));
    CHECK(move_list_contains(moves, Move{Square::H3, Square::E6, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves, Move{Square::H3, Square::F5, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves, Move{Square::H3, Square::G4, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves, Move{Square::H3, Square::H2, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves, Move{Square::H3, Square::H1, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves, Move{Square::H3, Square::H4, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves, Move{Square::H3, Square::H5, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves, Move{Square::H3, Square::H6, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves, Move{Square::H3, Square::H7, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves, Move{Square::H3, Square::H8, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves, Move{Square::H3, Square::G2, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves, Move{Square::H3, Square::F1, Piece::WhiteQueen}));

    CHECK_FALSE(move_list_contains(moves, Move{Square::D1, Square::D2, Piece::WhiteRook}));
    CHECK_FALSE(move_list_contains(moves, Move{Square::D1, Square::D3, Piece::WhiteRook}));
    CHECK_FALSE(move_list_contains(moves, Move{Square::D1, Square::D4, Piece::WhiteRook}));
    CHECK_FALSE(move_list_contains(moves, Move{Square::D1, Square::D5, Piece::WhiteRook}));
    CHECK_FALSE(move_list_contains(moves, Move{Square::D1, Square::D6, Piece::WhiteRook}));
    CHECK_FALSE(move_list_contains(moves, Move{Square::D1, Square::D7, Piece::WhiteRook, Piece::BlackBishop}));
    CHECK_FALSE(move_list_contains(moves, Move{Square::E3, Square::E4, Piece::WhitePawn}));
    CHECK_FALSE(move_list_contains(moves, Move{Square::G3, Square::F1, Piece::WhiteKnight}));
    CHECK_FALSE(move_list_contains(moves, Move{Square::G3, Square::H1, Piece::WhiteKnight}));
    CHECK_FALSE(move_list_contains(moves, Move{Square::G3, Square::E2, Piece::WhiteKnight}));
    CHECK_FALSE(move_list_contains(moves, Move{Square::G3, Square::E4, Piece::WhiteKnight}));
    CHECK_FALSE(move_list_contains(moves, Move{Square::G3, Square::F5, Piece::WhiteKnight}));
    CHECK_FALSE(move_list_contains(moves, Move{Square::G3, Square::H5, Piece::WhiteKnight}));
    CHECK_FALSE(move_list_contains(moves, Move{Square::G1, Square::F1, Piece::WhiteKing}));
    CHECK_FALSE(move_list_contains(moves, Move{Square::G1, Square::F2, Piece::WhiteKing}));
}

TEST_CASE("Position.Bitboard.MoveGeneration (2)", "[Position][MoveGeneration]") {
    Position position{FenString{"2k1r3/3p1N2/2b2pn1/q2N3P/1P2PQ2/P1R5/3B1P1p/R3K3 b - - 0 1"}};
    MoveList moves = position.all_legal_moves();
    CHECK(moves.size() == 33);

    CHECK(move_list_contains(moves, Move{Square::A5, Square::A8, Piece::BlackQueen}));
    CHECK(move_list_contains(moves, Move{Square::A5, Square::A7, Piece::BlackQueen}));
    CHECK(move_list_contains(moves, Move{Square::A5, Square::A6, Piece::BlackQueen}));
    CHECK(move_list_contains(moves, Move{Square::A5, Square::A4, Piece::BlackQueen}));
    CHECK(move_list_contains(moves, Move{Square::A5, Square::A3, Piece::BlackQueen, Piece::WhitePawn}));
    CHECK(move_list_contains(moves, Move{Square::A5, Square::B4, Piece::BlackQueen, Piece::WhitePawn}));
    CHECK(move_list_contains(moves, Move{Square::A5, Square::D8, Piece::BlackQueen}));
    CHECK(move_list_contains(moves, Move{Square::A5, Square::C7, Piece::BlackQueen}));
    CHECK(move_list_contains(moves, Move{Square::A5, Square::B6, Piece::BlackQueen}));
    CHECK(move_list_contains(moves, Move{Square::A5, Square::B5, Piece::BlackQueen}));
    CHECK(move_list_contains(moves, Move{Square::A5, Square::C5, Piece::BlackQueen}));
    CHECK(move_list_contains(moves, Move{Square::A5, Square::D5, Piece::BlackQueen, Piece::WhiteKnight}));

    CHECK(move_list_contains(moves, Move{Square::C8, Square::B7, Piece::BlackKing}));

    CHECK(move_list_contains(moves, Move{Square::D7, Square::D6, Piece::BlackPawn}));

    CHECK(move_list_contains(moves, Move{Square::E8, Square::D8, Piece::BlackRook}));
    CHECK(move_list_contains(moves, Move{Square::E8, Square::F8, Piece::BlackRook}));
    CHECK(move_list_contains(moves, Move{Square::E8, Square::G8, Piece::BlackRook}));
    CHECK(move_list_contains(moves, Move{Square::E8, Square::H8, Piece::BlackRook}));
    CHECK(move_list_contains(moves, Move{Square::E8, Square::E7, Piece::BlackRook}));
    CHECK(move_list_contains(moves, Move{Square::E8, Square::E6, Piece::BlackRook}));
    CHECK(move_list_contains(moves, Move{Square::E8, Square::E5, Piece::BlackRook}));
    CHECK(move_list_contains(moves, Move{Square::E8, Square::E4, Piece::BlackRook, Piece::WhitePawn}));

    CHECK(move_list_contains(moves, Move{Square::F6, Square::F5, Piece::BlackPawn}));

    CHECK(move_list_contains(moves, Move{Square::G6, Square::F8, Piece::BlackKnight}));
    CHECK(move_list_contains(moves, Move{Square::G6, Square::E7, Piece::BlackKnight}));
    CHECK(move_list_contains(moves, Move{Square::G6, Square::E5, Piece::BlackKnight}));
    CHECK(move_list_contains(moves, Move{Square::G6, Square::F4, Piece::BlackKnight, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves, Move{Square::G6, Square::H4, Piece::BlackKnight}));
    CHECK(move_list_contains(moves, Move{Square::G6, Square::H8, Piece::BlackKnight}));

    CHECK(move_list_contains_promotions(moves, Move{Square::H2, Square::H1, Piece::BlackPawn}));

    CHECK_FALSE(move_list_contains(moves, Move{Square::E8, Square::B8, Piece::BlackKing}));
    CHECK_FALSE(move_list_contains(moves, Move{Square::E8, Square::C7, Piece::BlackKing}));
    CHECK_FALSE(move_list_contains(moves, Move{Square::E8, Square::D8, Piece::BlackKing}));

    CHECK_FALSE(move_list_contains(moves, Move{Square::C6, Square::A8, Piece::BlackBishop}));
    CHECK_FALSE(move_list_contains(moves, Move{Square::C6, Square::B7, Piece::BlackBishop}));
    CHECK_FALSE(move_list_contains(moves, Move{Square::C6, Square::B5, Piece::BlackBishop}));
    CHECK_FALSE(move_list_contains(moves, Move{Square::C6, Square::A4, Piece::BlackBishop}));
    CHECK_FALSE(move_list_contains(moves, Move{Square::C6, Square::D5, Piece::BlackBishop, Piece::WhiteKnight}));
}
