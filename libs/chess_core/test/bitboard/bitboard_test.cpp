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

    // replace WhiteKnight with a WhitePawn
    bitboard.set_piece(Piece::WhitePawn, Square::A1);
    CHECK(bitboard.has_piece(Piece::WhitePawn));
    CHECK(bitboard.has_piece(PieceType::Pawn));
    CHECK(bitboard.has_piece(Color::White));
    CHECK_FALSE(bitboard.has_piece(Piece::WhiteKnight));
    CHECK_FALSE(bitboard.has_piece(PieceType::Knight));
    CHECK(bitboard.has_piece(Piece::BlackBishop));
    CHECK(bitboard.has_piece(PieceType::Bishop));
    CHECK(bitboard.has_piece(Color::Black));
}

TEST_CASE("Bitboard.Bitboard.Get", "[Bitboard][Init]") {
    Bitboard bitboard{};
    bitboard.set_piece(Piece::BlackBishop, Square::E2);
    bitboard.set_piece(Piece::WhiteKnight, Square::A1);

    CHECK(bitboard.has_piece(Square::E2));
    CHECK(bitboard.has_piece(Square::A1));
    CHECK_FALSE(bitboard.has_piece(Square::A2));
    CHECK_FALSE(bitboard.has_piece(Square::F3));
    CHECK_FALSE(bitboard.has_piece(Square::H7));

    const auto p1 = bitboard.get_piece(Square::E2);
    REQUIRE(p1.has_value());
    CHECK(p1 == Piece::BlackBishop);

    const auto p2 = bitboard.get_piece(Square::A1);
    REQUIRE(p2.has_value());
    CHECK(p2 == Piece::WhiteKnight);

    const auto p3 = bitboard.get_piece(Square::A2);
    CHECK_FALSE(p3.has_value());
}

TEST_CASE("Bitboard.Bitboard.Clear", "[Bitboard][Init]") {
    Bitboard bitboard{};
    bitboard.set_piece(Piece::BlackBishop, Square::E2);
    bitboard.set_piece(Piece::WhiteKnight, Square::A1);
    bitboard.set_piece(Piece::WhiteRook, Square::F3);

    bitboard.clear_square(Square::E2);
    CHECK_FALSE(bitboard.has_piece(Square::E2));
    CHECK(bitboard.has_piece(Square::A1));
    CHECK(bitboard.has_piece(Square::F3));

    bitboard.clear_square(Square::A1);
    CHECK_FALSE(bitboard.has_piece(Square::E2));
    CHECK_FALSE(bitboard.has_piece(Square::A1));
    CHECK(bitboard.has_piece(Square::F3));

    bitboard.clear_square(Square::F3);
    CHECK_FALSE(bitboard.has_piece(Square::E2));
    CHECK_FALSE(bitboard.has_piece(Square::A1));
    CHECK_FALSE(bitboard.has_piece(Square::F3));
    CHECK(bitboard.empty());
}

TEST_CASE("Bitboard.Bitboard.FromFEN", "[Bitboard][Init]") {
    Bitboard empty_bitboard{FenString{}};
    CHECK(empty_bitboard.empty());
    CHECK_FALSE(empty_bitboard.has_piece(PieceType::Pawn));
    CHECK_FALSE(empty_bitboard.has_piece(Piece::WhiteKnight));
    CHECK_FALSE(empty_bitboard.has_piece(Color::Black));

    Bitboard start_cfg_bitboard{FenString{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"}};
    CHECK_FALSE(start_cfg_bitboard.empty());
    CHECK(start_cfg_bitboard.get_piece(Square::A1) == Piece::WhiteRook);
    CHECK(start_cfg_bitboard.get_piece(Square::B1) == Piece::WhiteKnight);
    CHECK(start_cfg_bitboard.get_piece(Square::C1) == Piece::WhiteBishop);
    CHECK(start_cfg_bitboard.get_piece(Square::D1) == Piece::WhiteQueen);
    CHECK(start_cfg_bitboard.get_piece(Square::E1) == Piece::WhiteKing);
    CHECK(start_cfg_bitboard.get_piece(Square::F1) == Piece::WhiteBishop);
    CHECK(start_cfg_bitboard.get_piece(Square::G1) == Piece::WhiteKnight);
    CHECK(start_cfg_bitboard.get_piece(Square::H1) == Piece::WhiteRook);

    CHECK(start_cfg_bitboard.get_piece(Square::A2) == Piece::WhitePawn);
    CHECK(start_cfg_bitboard.get_piece(Square::B2) == Piece::WhitePawn);
    CHECK(start_cfg_bitboard.get_piece(Square::C2) == Piece::WhitePawn);
    CHECK(start_cfg_bitboard.get_piece(Square::H2) == Piece::WhitePawn);

    CHECK_FALSE(start_cfg_bitboard.get_piece(Square::A3).has_value());
    CHECK_FALSE(start_cfg_bitboard.get_piece(Square::B4).has_value());
    CHECK_FALSE(start_cfg_bitboard.get_piece(Square::D5).has_value());
    CHECK_FALSE(start_cfg_bitboard.get_piece(Square::F6).has_value());

    CHECK(start_cfg_bitboard.get_piece(Square::A7) == Piece::BlackPawn);
    CHECK(start_cfg_bitboard.get_piece(Square::B7) == Piece::BlackPawn);
    CHECK(start_cfg_bitboard.get_piece(Square::F7) == Piece::BlackPawn);
    CHECK(start_cfg_bitboard.get_piece(Square::H7) == Piece::BlackPawn);

    CHECK(start_cfg_bitboard.get_piece(Square::A8) == Piece::BlackRook);
    CHECK(start_cfg_bitboard.get_piece(Square::B8) == Piece::BlackKnight);
    CHECK(start_cfg_bitboard.get_piece(Square::C8) == Piece::BlackBishop);
    CHECK(start_cfg_bitboard.get_piece(Square::D8) == Piece::BlackQueen);
    CHECK(start_cfg_bitboard.get_piece(Square::E8) == Piece::BlackKing);
    CHECK(start_cfg_bitboard.get_piece(Square::F8) == Piece::BlackBishop);
    CHECK(start_cfg_bitboard.get_piece(Square::G8) == Piece::BlackKnight);
    CHECK(start_cfg_bitboard.get_piece(Square::H8) == Piece::BlackRook);
}
