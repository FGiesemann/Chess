/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/bitboard.h"

using namespace chesscore;

TEST_CASE("Bitboard.Bitboard.Empty", "[Bitboard][Init]") {
    Bitboard bitboard{};
    CHECK(bitboard.empty());
    CHECK_FALSE(bitboard.has_piece(PieceType::Pawn));
    CHECK_FALSE(bitboard.has_piece(Piece::WhiteKnight));
}

TEST_CASE("Bitboard.Bitboard.Set", "[Bitboard][Init]") {
    Bitboard bitboard{};
    bitboard.set_piece(Piece::BlackBishop, Square::E2);

    CHECK_FALSE(bitboard.empty());
    CHECK(bitboard.has_piece(PieceType::Bishop));
    CHECK(bitboard.has_piece(Piece::BlackBishop));
    CHECK_FALSE(bitboard.has_piece(PieceType::Pawn));
    CHECK_FALSE(bitboard.has_piece(Piece::WhiteBishop));
    CHECK(bitboard.has_piece(Color::Black));
    CHECK_FALSE(bitboard.has_piece(Color::White));
}
