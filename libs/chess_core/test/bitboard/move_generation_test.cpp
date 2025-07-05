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

TEST_CASE("Bitboard.Bitboard.MoveGeneration.Knight", "[Bitboard][MoveGeneration]") {
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
}
