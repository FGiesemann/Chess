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
    CHECK_FALSE(bitboard.has_piece(Color::White));
}

TEST_CASE("Bitboard.Bitboard.Set", "[Bitboard][Init]") {
    Bitboard bitboard{};
    bitboard.set_piece(Piece::BlackBishop, Square::E2);

    CHECK_FALSE(bitboard.empty());
    CHECK(bitboard.has_piece(Piece::BlackBishop));
    CHECK(bitboard.has_piece(PieceType::Bishop));
    CHECK(bitboard.has_piece(Color::Black));

    CHECK_FALSE(bitboard.has_piece(PieceType::Pawn));
    CHECK_FALSE(bitboard.has_piece(Piece::WhiteBishop));
    CHECK_FALSE(bitboard.has_piece(Color::White));

    bitboard.set_piece(Piece::WhiteKnight, Square::A1);
    CHECK(bitboard.has_piece(Piece::WhiteKnight));
    CHECK(bitboard.has_piece(PieceType::Knight));
    CHECK(bitboard.has_piece(Color::White));

    CHECK_FALSE(bitboard.has_piece(PieceType::Pawn));
    CHECK_FALSE(bitboard.has_piece(Piece::WhiteBishop));
    CHECK(bitboard.has_piece(Color::Black));
    CHECK(bitboard.has_piece(Piece::BlackBishop));
}

TEST_CASE("Bitboard.Bitboard.Get", "[Bitboard][Init]") {
    Bitboard bitboard{};
    bitboard.set_piece(Piece::BlackBishop, Square::E2);
    bitboard.set_piece(Piece::WhiteKnight, Square::A1);

    const auto p1 = bitboard.get_piece(Square::E2);
    REQUIRE(p1.has_value());
    CHECK(p1 == Piece::BlackBishop);

    const auto p2 = bitboard.get_piece(Square::A1);
    REQUIRE(p2.has_value());
    CHECK(p2 == Piece::WhiteKnight);

    const auto p3 = bitboard.get_piece(Square::A2);
    CHECK_FALSE(p3.has_value());
}
