/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/bitboard.h"
#include "chesscore/fen.h"
#include "chesscore/move.h"

using namespace chesscore;

TEST_CASE("Bitboard.Bitboard.MakeMove", "[Bitboard][MakeMove]") {
    Bitboard board{FenString::starting_position()};

    Move m1{
        .from = Square::E2,
        .to = Square::E4,
        .piece = Piece::WhitePawn,
        .captured{},
        .promoted{},
        .en_passant{Square::E3},
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
        .captured{},
        .promoted{},
        .en_passant{Square::D6},
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
        .captured{},
        .promoted{},
        .en_passant{},
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
        .captured{},
        .promoted{},
        .en_passant{},
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
        .captured{},
        .promoted{},
        .en_passant{},
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
        .promoted{},
        .en_passant{},
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
        .captured{},
        .promoted{},
        .en_passant{},
        .castling_rights_before{CastlingRights::all()},
        .halfmove_clock_before = 0,
    };
    board.make_move(m7);
    CHECK_FALSE(board.get_piece(Square::E1).has_value());
    CHECK(board.get_piece(Square::G1) == Piece::WhiteKing);
    CHECK_FALSE(board.get_piece(Square::H1).has_value());
    CHECK(board.get_piece(Square::F1) == Piece::WhiteRook);
}
