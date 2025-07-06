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

TEST_CASE("Bitboard.Bitboard.MoveGeneration.Knight (no check)", "[Bitboard][MoveGeneration]") {
    Position<Bitboard> position1{FenString{"2k1n3/6N1/8/1P1b1r2/3P4/2N5/4q3/KP6 w - - 0 1"}};
    MoveList moves1{};
    position1.board().all_knight_moves(moves1, position1.state());
    CHECK(moves1.size() == 10);
    CHECK(move_list_contains(moves1, Move{Square::C3, Square::A4, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves1, Move{Square::C3, Square::D5, Piece::WhiteKnight, Piece::BlackBishop}));
    CHECK(move_list_contains(moves1, Move{Square::C3, Square::E4, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves1, Move{Square::C3, Square::E2, Piece::WhiteKnight, Piece::BlackQueen}));
    CHECK(move_list_contains(moves1, Move{Square::C3, Square::D1, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves1, Move{Square::C3, Square::A2, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves1, Move{Square::G7, Square::E8, Piece::WhiteKnight, Piece::BlackKnight}));
    CHECK(move_list_contains(moves1, Move{Square::G7, Square::H5, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves1, Move{Square::G7, Square::F5, Piece::WhiteKnight, Piece::BlackRook}));
    CHECK(move_list_contains(moves1, Move{Square::G7, Square::E6, Piece::WhiteKnight}));

    Position<Bitboard> position2{FenString{"8/2p5/1P3q2/3n4/5P1r/2N1B3/6n1/8 b - - 0 1"}};
    MoveList moves2{};
    position2.board().all_knight_moves(moves2, position2.state());
    CHECK(moves2.size() == 9);
    CHECK(move_list_contains(moves2, Move{Square::D5, Square::E7, Piece::BlackKnight}));
    CHECK(move_list_contains(moves2, Move{Square::D5, Square::F4, Piece::BlackKnight, Piece::WhitePawn}));
    CHECK(move_list_contains(moves2, Move{Square::D5, Square::E3, Piece::BlackKnight, Piece::WhiteBishop}));
    CHECK(move_list_contains(moves2, Move{Square::D5, Square::C3, Piece::BlackKnight, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves2, Move{Square::D5, Square::B4, Piece::BlackKnight}));
    CHECK(move_list_contains(moves2, Move{Square::D5, Square::B6, Piece::BlackKnight, Piece::WhitePawn}));
    CHECK(move_list_contains(moves2, Move{Square::G2, Square::F4, Piece::BlackKnight, Piece::WhitePawn}));
    CHECK(move_list_contains(moves2, Move{Square::G2, Square::E1, Piece::BlackKnight}));
    CHECK(move_list_contains(moves2, Move{Square::G2, Square::E3, Piece::BlackKnight, Piece::WhiteBishop}));
}

TEST_CASE("Bitboard.Bitboard.MoveGeneration.King (no check)", "[Bitboard][MoveGeneration]") {
    Position<Bitboard> position1{FenString{"8/1b1P4/2k5/8/5P2/5K2/4p3/8 w - - 0 1"}};
    MoveList moves1{};
    position1.board().all_king_moves(moves1, position1.state());
    CHECK(moves1.size() == 7);
    CHECK(move_list_contains(moves1, Move{Square::F3, Square::E4, Piece::WhiteKing}));
    CHECK(move_list_contains(moves1, Move{Square::F3, Square::G4, Piece::WhiteKing}));
    CHECK(move_list_contains(moves1, Move{Square::F3, Square::G3, Piece::WhiteKing}));
    CHECK(move_list_contains(moves1, Move{Square::F3, Square::G2, Piece::WhiteKing}));
    CHECK(move_list_contains(moves1, Move{Square::F3, Square::F2, Piece::WhiteKing}));
    CHECK(move_list_contains(moves1, Move{Square::F3, Square::E2, Piece::WhiteKing, Piece::BlackPawn}));
    CHECK(move_list_contains(moves1, Move{Square::F3, Square::E3, Piece::WhiteKing}));

    Position<Bitboard> position2{FenString{"8/1b1P4/2k5/8/5P2/5K2/4p3/8 b - - 0 1"}};
    MoveList moves2{};
    position2.board().all_king_moves(moves2, position2.state());
    CHECK(moves2.size() == 7);
    CHECK(move_list_contains(moves2, Move{Square::C6, Square::C7, Piece::BlackKing}));
    CHECK(move_list_contains(moves2, Move{Square::C6, Square::D7, Piece::BlackKing, Piece::WhitePawn}));
    CHECK(move_list_contains(moves2, Move{Square::C6, Square::D6, Piece::BlackKing}));
    CHECK(move_list_contains(moves2, Move{Square::C6, Square::D5, Piece::BlackKing}));
    CHECK(move_list_contains(moves2, Move{Square::C6, Square::C5, Piece::BlackKing}));
    CHECK(move_list_contains(moves2, Move{Square::C6, Square::B5, Piece::BlackKing}));
    CHECK(move_list_contains(moves2, Move{Square::C6, Square::B6, Piece::BlackKing}));
}

TEST_CASE("Bitboard.Bitboard.MoveGeneration.SlidingPieces.Ray N", "[Bitboard][MoveGeneration]") {
    Position<Bitboard> position1{FenString{"8/5b2/2p1P2P/8/1pP2Rp1/6p1/3b4/8 w - - 0 1"}};
    MoveList moves1{};
    position1.board().all_moves_along_ray(Piece::WhiteRook, Square::F4, RayDirection::North, moves1, position1.state());
    CHECK(moves1.size() == 3);
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::F5, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::F6, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::F7, Piece::WhiteRook, Piece::BlackBishop}));
}

TEST_CASE("Bitboard.Bitboard.MoveGeneration.SlidingPieces.Ray W", "[Bitboard][MoveGeneration]") {
    Position<Bitboard> position1{FenString{"8/5b2/2p1P2P/8/1pP2Rp1/6p1/3b4/8 w - - 0 1"}};
    MoveList moves1{};
    position1.board().all_moves_along_ray(Piece::WhiteRook, Square::F4, RayDirection::West, moves1, position1.state());
    CHECK(moves1.size() == 2);
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::E4, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::D4, Piece::WhiteRook}));
}

TEST_CASE("Bitboard.Bitboard.MoveGeneration.SlidingPieces.Rook (no check)", "[Bitboard][MoveGeneration]") {
    Position<Bitboard> position1{FenString{"8/5b2/2p1P2P/8/1pP2Rp1/6p1/3b4/8 w - - 0 1"}};
    MoveList moves1{};
    position1.board().all_sliding_moves(Piece::WhiteRook, Square::F4, moves1, position1.state());
    CHECK(moves1.size() == 9);
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::F1, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::F2, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::F3, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::G4, Piece::WhiteRook, Piece::BlackPawn}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::F5, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::F6, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::F7, Piece::WhiteRook, Piece::BlackBishop}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::E4, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::D4, Piece::WhiteRook}));
}
