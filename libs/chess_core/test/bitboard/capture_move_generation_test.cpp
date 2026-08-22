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

TEST_CASE("Bitboard.Bitboard.CaptureMoveGeneration.Starting Position", "[Bitboard][MoveGeneration]") {
    Position position{FenString::starting_position()};
    MoveList moves = position.board().capture_moves(position.state());
    CHECK(moves.empty());
}

TEST_CASE("Bitboard.Bitboard.CaptureMoveGeneration.No Captures", "[Bitboard][MoveGeneration]") {
    Position position1{FenString{"rn1q1b1k/p3p1r1/2n5/5B2/2N2P1N/1P6/P1R5/R3KQ2 w - - 0 1"}};
    MoveList moves = position1.board().capture_moves(position1.state());
    CHECK(moves.empty());

    Position position2{FenString{"rn1q1b1k/p3p1r1/2n5/5B2/2N2P1N/1P6/P1R5/R3KQ2 b - - 0 1"}};
    moves = position2.board().capture_moves(position2.state());
    CHECK(moves.empty());
}

TEST_CASE("Bitboard.Bitboard.CaptureMoveGeneration.Captures", "[Bitboard][MoveGeneration]") {
    Position position1{FenString{"rn1q3k/p3pb1r/2n5/5Bp1/1N3P1N/1P3Q2/P1RP4/R3K3 w - - 0 1"}};
    MoveList moves = position1.board().capture_moves(position1.state());
    CHECK(moves.size() == 5);
    CHECK(move_list_contains(moves, Move{Square::B4, Square::C6, Piece::WhiteKnight, Piece::BlackKnight}));
    CHECK(move_list_contains(moves, Move{Square::C2, Square::C6, Piece::WhiteRook, Piece::BlackKnight}));
    CHECK(move_list_contains(moves, Move{Square::F3, Square::C6, Piece::WhiteQueen, Piece::BlackKnight}));
    CHECK(move_list_contains(moves, Move{Square::F5, Square::H7, Piece::WhiteBishop, Piece::BlackRook}));
    CHECK(move_list_contains(moves, Move{Square::F4, Square::G5, Piece::WhitePawn, Piece::BlackPawn}));

    Position position2{FenString{"rn1q3k/p3pb1r/2n5/5Bp1/1N3P1N/1P3Q2/P1RP4/R3K3 b - - 0 1"}};
    moves = position2.board().capture_moves(position2.state());
    CHECK(moves.size() == 6);
    CHECK(move_list_contains(moves, Move{Square::C6, Square::B4, Piece::BlackKnight, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves, Move{Square::D8, Square::D2, Piece::BlackQueen, Piece::WhitePawn}));
    CHECK(move_list_contains(moves, Move{Square::F7, Square::B3, Piece::BlackBishop, Piece::WhitePawn}));
    CHECK(move_list_contains(moves, Move{Square::G5, Square::F4, Piece::BlackPawn, Piece::WhitePawn}));
    CHECK(move_list_contains(moves, Move{Square::G5, Square::H4, Piece::BlackPawn, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves, Move{Square::H7, Square::H4, Piece::BlackRook, Piece::WhiteKnight}));
}

TEST_CASE("Bitboard.Bitboard.CaptureMoveGeneration.En Passant", "[Bitboard][MoveGeneration]") {
    Position position{FenString{"8/8/8/8/4Pp2/8/8/8 b - e3 0 1"}};
    MoveList moves = position.board().capture_moves(position.state());
    CHECK(moves.size() == 1);
    CHECK(move_list_contains(moves, Move{Square::F4, Square::E3, Piece::BlackPawn, Piece::WhitePawn, true}));
}
