/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/piece.h"

using namespace chesscore;

TEST_CASE("Data.Piece.Type from Index", "[Piece]") {
    CHECK(piece_type_from_index(0) == PieceType::Pawn);
    CHECK(piece_type_from_index(1) == PieceType::Rook);
    CHECK(piece_type_from_index(2) == PieceType::Knight);
    CHECK(piece_type_from_index(3) == PieceType::Bishop);
    CHECK(piece_type_from_index(4) == PieceType::Queen);
    CHECK(piece_type_from_index(5) == PieceType::King);
    CHECK_THROWS_AS(piece_type_from_index(6), ChessException);
}

TEST_CASE("Data.Piece.Type from Char", "[Piece]") {
    CHECK(piece_type_from_char('r') == PieceType::Rook);
    CHECK(piece_type_from_char('n') == PieceType::Knight);
    CHECK(piece_type_from_char('b') == PieceType::Bishop);
    CHECK(piece_type_from_char('q') == PieceType::Queen);
    CHECK(piece_type_from_char('k') == PieceType::King);
    CHECK(piece_type_from_char('p') == PieceType::Pawn);
    CHECK_THROWS_AS(piece_type_from_char('a'), ChessException);
}
