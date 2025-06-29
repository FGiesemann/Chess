/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/bitboard.h"
#include "chesscore/fen.h"
#include "chesscore/move.h"

using namespace chesscore;

TEST_CASE("Bitboard.Bitboard.MakeMove.General", "[Bitboard][MakeMove]") {
    Bitboard board{FenString::starting_position()};

    Move m1{
        .from = Square::E2,
        .to = Square::E4,
        .piece = Piece::WhitePawn,
        .castling_rights_before{CastlingRights::all()},
        .halfmove_clock_before = 0,
    };
    board.make_move(m1);

    CHECK_FALSE(board.get_piece(Square::E2).has_value());
    CHECK(board.get_piece(Square::E4) == Piece::WhitePawn);

    Move m2{
        .from = Square::D7,
        .to = Square::D5,
        .piece = Piece::BlackPawn,
        .castling_rights_before{CastlingRights::all()},
        .halfmove_clock_before = 0,
    };
    board.make_move(m2);
    CHECK_FALSE(board.get_piece(Square::D7).has_value());
    CHECK(board.get_piece(Square::D5) == Piece::BlackPawn);

    Move m3{
        .from = Square::F1,
        .to = Square::C4,
        .piece = Piece::WhiteBishop,
        .castling_rights_before{CastlingRights::all()},
        .halfmove_clock_before = 0,
    };
    board.make_move(m3);
    CHECK_FALSE(board.get_piece(Square::F1).has_value());
    CHECK(board.get_piece(Square::C4) == Piece::WhiteBishop);

    Move m4{
        .from = Square::E8,
        .to = Square::D7,
        .piece = Piece::BlackKing,
        .castling_rights_before{CastlingRights::all()},
        .halfmove_clock_before = 0,
    };
    board.make_move(m4);
    CHECK_FALSE(board.get_piece(Square::E8).has_value());
    CHECK(board.get_piece(Square::D7) == Piece::BlackKing);

    Move m5{
        .from = Square::G1,
        .to = Square::F3,
        .piece = Piece::WhiteKnight,
        .castling_rights_before{CastlingRights::all()},
        .halfmove_clock_before = 0,
    };
    board.make_move(m5);
    CHECK_FALSE(board.get_piece(Square::G1).has_value());
    CHECK(board.get_piece(Square::F3) == Piece::WhiteKnight);

    Move m6{
        .from = Square::D5,
        .to = Square::C4,
        .piece = Piece::BlackPawn,
        .captured{Piece::WhiteBishop},
        .castling_rights_before{CastlingRights::all()},
        .halfmove_clock_before = 0,
    };
    board.make_move(m6);
    CHECK_FALSE(board.get_piece(Square::D5).has_value());
    CHECK(board.get_piece(Square::C4) == Piece::BlackPawn);

    Move m7{
        .from = Square::E1,
        .to = Square::G1,
        .piece = Piece::WhiteKing,
        .castling_rights_before{CastlingRights::all()},
        .halfmove_clock_before = 0,
    };
    board.make_move(m7);
    CHECK_FALSE(board.get_piece(Square::E1).has_value());
    CHECK(board.get_piece(Square::G1) == Piece::WhiteKing);
    CHECK_FALSE(board.get_piece(Square::H1).has_value());
    CHECK(board.get_piece(Square::F1) == Piece::WhiteRook);
}

TEST_CASE("Bitboard.Bitboard.MakeMove.Promotion", "[Bitboard][MakeMove]") {
    Bitboard board{FenString{"2kr4/p4P1p/1p6/8/2B3b1/2N2N2/PP1P2PP/2B1K3 w - - 0 1"}};

    Move m{
        .from = Square::F7,
        .to = Square::F8,
        .piece = Piece::WhitePawn,
        .promoted{Piece::WhiteQueen},
        .castling_rights_before{CastlingRights::none()},
        .halfmove_clock_before = 0,
    };
    board.make_move(m);
    CHECK_FALSE(board.get_piece(Square::F7).has_value());
    CHECK(board.get_piece(Square::F8) == Piece::WhiteQueen);
}

TEST_CASE("Bitboard.Bitboard.MakeMove.Castling", "[Bitboard][MakeMove]") {
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
    CHECK_FALSE(board1.get_piece(Square::E1).has_value());
    CHECK(board1.get_piece(Square::G1) == Piece::WhiteKing);
    CHECK_FALSE(board1.get_piece(Square::H1).has_value());
    CHECK(board1.get_piece(Square::F1) == Piece::WhiteRook);

    board2.make_move(white_queenside_castling);
    CHECK_FALSE(board2.get_piece(Square::E1).has_value());
    CHECK(board2.get_piece(Square::C1) == Piece::WhiteKing);
    CHECK_FALSE(board2.get_piece(Square::A1).has_value());
    CHECK(board2.get_piece(Square::D1) == Piece::WhiteRook);

    board3.make_move(black_kingside_castling);
    CHECK_FALSE(board3.get_piece(Square::E8).has_value());
    CHECK(board3.get_piece(Square::G8) == Piece::BlackKing);
    CHECK_FALSE(board3.get_piece(Square::H8).has_value());
    CHECK(board3.get_piece(Square::F8) == Piece::BlackRook);

    board4.make_move(black_queenside_castling);
    CHECK_FALSE(board4.get_piece(Square::E8).has_value());
    CHECK(board4.get_piece(Square::C8) == Piece::BlackKing);
    CHECK_FALSE(board4.get_piece(Square::A8).has_value());
    CHECK(board4.get_piece(Square::D8) == Piece::BlackRook);
}
