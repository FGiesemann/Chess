/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/bitboard.h"
#include "chesscore/fen.h"
#include "chesscore/move.h"

using namespace chesscore;

TEST_CASE("Bitboard.Bitboard.UnmakeMove.SingleMove", "[Bitboard][UnmakeMove]") {
    Bitboard board{FenString::starting_position()};

    Move m{.from = Square::E2, .to = Square::E4, .piece = Piece::WhitePawn, .castling_rights_before{CastlingRights::all()}, .halfmove_clock_before = 0};
    board.make_move(m);

    CHECK_FALSE(board.get_piece(Square::E2).has_value());
    CHECK(board.get_piece(Square::E4) == Piece::WhitePawn);

    board.unmake_move(m);
    CHECK(board.get_piece(Square::E2) == Piece::WhitePawn);
    CHECK_FALSE(board.get_piece(Square::E4).has_value());
}

TEST_CASE("Bitboard.Bitboard.UnmakeMove.Capture", "[Bitboard][UnmakeMove]") {
    Bitboard board{FenString{"8/8/8/3p4/2B1P3/8/8/8 b - - 0 1"}};

    Move m{
        .from = Square::D5, .to = Square::C4, .piece = Piece::BlackPawn, .captured{Piece::WhiteBishop}, .castling_rights_before{CastlingRights::all()}, .halfmove_clock_before = 0
    };
    board.make_move(m);
    board.unmake_move(m);
    CHECK(board.get_piece(Square::D5) == Piece::BlackPawn);
    CHECK(board.get_piece(Square::C4) == Piece::WhiteBishop);
}

TEST_CASE("Bitboard.Bitboard.UnmakeMove.EnPassant", "[Bitboard][UnmakeMove]") {
    Bitboard board{FenString{"8/8/8/8/4Pp2/8/8/8 b - e3 0 1"}};

    Move m{
        .from = Square::F4,
        .to = Square::E3,
        .piece = Piece::BlackPawn,
        .captured{Piece::WhitePawn},
        .capturing_en_passant = true,
        .castling_rights_before{CastlingRights::all()},
        .halfmove_clock_before = 0
    };
    board.make_move(m);
    CHECK_FALSE(board.get_piece(Square::F4).has_value());
    CHECK(board.get_piece(Square::E3) == Piece::BlackPawn);
    CHECK_FALSE(board.get_piece(Square::E4).has_value());

    board.unmake_move(m);
    CHECK(board.get_piece(Square::F4) == Piece::BlackPawn);
    CHECK(board.get_piece(Square::E4) == Piece::WhitePawn);
    CHECK_FALSE(board.get_piece(Square::E3).has_value());
}
