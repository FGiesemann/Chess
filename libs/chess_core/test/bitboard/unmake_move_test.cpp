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

TEST_CASE("Bitboard.Bitboard.UnmakeMove.Castling", "[Bitboard][UnmakeMove]") {
    FenString fen{"r3k2r/ppp2ppp/8/8/8/8/PPP2PPP/R3K2R w KQkq - 0 1"};
    Bitboard board1{fen};
    Bitboard board2{fen};
    Bitboard board3{fen};
    Bitboard board4{fen};

    Move white_kingside_castling{
        .from = Square::E1,
        .to = Square::G1,
        .piece = Piece::WhiteKing,
        .castling_rights_before{CastlingRights::all()},
        .halfmove_clock_before = 0,
    };
    Move white_queenside_castling{
        .from = Square::E1,
        .to = Square::C1,
        .piece = Piece::WhiteKing,
        .castling_rights_before{CastlingRights::all()},
        .halfmove_clock_before = 0,
    };
    Move black_kingside_castling{
        .from = Square::E8,
        .to = Square::G8,
        .piece = Piece::BlackKing,
        .castling_rights_before{CastlingRights::all()},
        .halfmove_clock_before = 0,
    };
    Move black_queenside_castling{
        .from = Square::E8,
        .to = Square::C8,
        .piece = Piece::BlackKing,
        .castling_rights_before{CastlingRights::all()},
        .halfmove_clock_before = 0,
    };

    board1.make_move(white_kingside_castling);
    board1.unmake_move(white_kingside_castling);
    CHECK(board1.get_piece(Square::E1) == Piece::WhiteKing);
    CHECK_FALSE(board1.get_piece(Square::G1).has_value());
    CHECK(board1.get_piece(Square::H1) == Piece::WhiteRook);
    CHECK_FALSE(board1.get_piece(Square::F1).has_value());

    board2.make_move(white_queenside_castling);
    board2.unmake_move(white_queenside_castling);
    CHECK(board2.get_piece(Square::E1) == Piece::WhiteKing);
    CHECK_FALSE(board2.get_piece(Square::C1).has_value());
    CHECK(board2.get_piece(Square::A1) == Piece::WhiteRook);
    CHECK_FALSE(board2.get_piece(Square::D1).has_value());

    board3.make_move(black_kingside_castling);
    board3.unmake_move(black_kingside_castling);
    CHECK(board3.get_piece(Square::E8) == Piece::BlackKing);
    CHECK_FALSE(board3.get_piece(Square::G8).has_value());
    CHECK(board3.get_piece(Square::H8) == Piece::BlackRook);
    CHECK_FALSE(board3.get_piece(Square::F8).has_value());

    board4.make_move(black_queenside_castling);
    board4.unmake_move(black_queenside_castling);
    CHECK(board4.get_piece(Square::E8) == Piece::BlackKing);
    CHECK_FALSE(board4.get_piece(Square::C8).has_value());
    CHECK(board4.get_piece(Square::A8) == Piece::BlackRook);
    CHECK_FALSE(board4.get_piece(Square::D8).has_value());
}

