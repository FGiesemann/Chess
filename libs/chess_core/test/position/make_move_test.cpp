/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/bitboard.h"
#include "chesscore/position.h"

using namespace chesscore;

TEST_CASE("Position.MakeMove", "[Position][MakeMove]") {
    Position<Bitboard> position{FenString::starting_position()};

    Move m1{
        .from = Square::E2,
        .to = Square::E4,
        .piece = Piece::WhitePawn,
        .castling_rights_before{position.castling_rights()},
        .halfmove_clock_before = position.halfmove_clock(),
        .en_passant_target_before = position.en_passant_target(),
    };

    position.make_move(m1);
    CHECK_FALSE(position.board().get_piece(Square::E2).has_value());
    CHECK(position.board().get_piece(Square::E4) == Piece::WhitePawn);
    CHECK(position.castling_rights() == CastlingRights::all());
    CHECK(position.side_to_move() == Color::Black);
    CHECK(position.en_passant_target() == Square::E3);
    CHECK(position.halfmove_clock() == 0);
    CHECK(position.fullmove_number() == 1);

    Move m2{
        .from = Square::D7,
        .to = Square::D5,
        .piece = Piece::BlackPawn,
        .castling_rights_before{position.castling_rights()},
        .halfmove_clock_before = position.halfmove_clock(),
        .en_passant_target_before = position.en_passant_target(),
    };

    position.make_move(m2);
    CHECK_FALSE(position.board().get_piece(Square::D7).has_value());
    CHECK(position.board().get_piece(Square::D5) == Piece::BlackPawn);
    CHECK(position.castling_rights() == CastlingRights::all());
    CHECK(position.side_to_move() == Color::White);
    CHECK(position.en_passant_target() == Square::D6);
    CHECK(position.halfmove_clock() == 0);
    CHECK(position.fullmove_number() == 2);

    Move m3{
        .from = Square::F1,
        .to = Square::C4,
        .piece = Piece::WhiteBishop,
        .castling_rights_before{position.castling_rights()},
        .halfmove_clock_before = position.halfmove_clock(),
        .en_passant_target_before = position.en_passant_target(),
    };

    position.make_move(m3);
    CHECK_FALSE(position.board().get_piece(Square::F1).has_value());
    CHECK(position.board().get_piece(Square::C4) == Piece::WhiteBishop);
    CHECK(position.castling_rights() == CastlingRights::all());
    CHECK(position.side_to_move() == Color::Black);
    CHECK_FALSE(position.en_passant_target().has_value());
    CHECK(position.halfmove_clock() == 1);
    CHECK(position.fullmove_number() == 2);

    Move m4{
        .from = Square::E8,
        .to = Square::D7,
        .piece = Piece::BlackKing,
        .castling_rights_before{position.castling_rights()},
        .halfmove_clock_before = position.halfmove_clock(),
        .en_passant_target_before = position.en_passant_target(),
    };

    position.make_move(m4);
    CHECK_FALSE(position.board().get_piece(Square::E8).has_value());
    CHECK(position.board().get_piece(Square::D7) == Piece::BlackKing);
    CHECK(position.castling_rights()['K']);
    CHECK(position.castling_rights()['Q']);
    CHECK_FALSE(position.castling_rights()['k']);
    CHECK_FALSE(position.castling_rights()['q']);
    CHECK(position.side_to_move() == Color::White);
    CHECK_FALSE(position.en_passant_target().has_value());
    CHECK(position.halfmove_clock() == 2);
    CHECK(position.fullmove_number() == 3);

    Move m5{
        .from = Square::G1,
        .to = Square::F3,
        .piece = Piece::WhiteKnight,
        .castling_rights_before{position.castling_rights()},
        .halfmove_clock_before = position.halfmove_clock(),
        .en_passant_target_before = position.en_passant_target(),
    };

    position.make_move(m5);
    CHECK_FALSE(position.board().get_piece(Square::G1).has_value());
    CHECK(position.board().get_piece(Square::F3) == Piece::WhiteKnight);
    CHECK(position.castling_rights()['K']);
    CHECK(position.castling_rights()['Q']);
    CHECK_FALSE(position.castling_rights()['k']);
    CHECK_FALSE(position.castling_rights()['q']);
    CHECK(position.side_to_move() == Color::Black);
    CHECK_FALSE(position.en_passant_target().has_value());
    CHECK(position.halfmove_clock() == 3);
    CHECK(position.fullmove_number() == 3);

    Move m6{
        .from = Square::D5,
        .to = Square::C4,
        .piece = Piece::BlackPawn,
        .captured{Piece::WhiteBishop},
        .castling_rights_before{position.castling_rights()},
        .halfmove_clock_before = position.halfmove_clock(),
        .en_passant_target_before = position.en_passant_target(),
    };

    position.make_move(m6);
    CHECK_FALSE(position.board().get_piece(Square::D5).has_value());
    CHECK(position.board().get_piece(Square::C4) == Piece::BlackPawn);
    CHECK(position.castling_rights()['K']);
    CHECK(position.castling_rights()['Q']);
    CHECK_FALSE(position.castling_rights()['k']);
    CHECK_FALSE(position.castling_rights()['q']);
    CHECK(position.side_to_move() == Color::White);
    CHECK_FALSE(position.en_passant_target().has_value());
    CHECK(position.halfmove_clock() == 0);
    CHECK(position.fullmove_number() == 4);

    Move m7{
        .from = Square::E1,
        .to = Square::G1,
        .piece = Piece::WhiteKing,
        .castling_rights_before{position.castling_rights()},
        .halfmove_clock_before = position.halfmove_clock(),
        .en_passant_target_before = position.en_passant_target(),
    };

    position.make_move(m7);
    CHECK_FALSE(position.board().get_piece(Square::E1).has_value());
    CHECK(position.board().get_piece(Square::G1) == Piece::WhiteKing);
    CHECK_FALSE(position.board().get_piece(Square::H1).has_value());
    CHECK(position.board().get_piece(Square::F1) == Piece::WhiteRook);
    CHECK_FALSE(position.castling_rights()['K']);
    CHECK_FALSE(position.castling_rights()['Q']);
    CHECK_FALSE(position.castling_rights()['k']);
    CHECK_FALSE(position.castling_rights()['q']);
    CHECK(position.side_to_move() == Color::Black);
    CHECK_FALSE(position.en_passant_target().has_value());
    CHECK(position.halfmove_clock() == 1);
    CHECK(position.fullmove_number() == 4);
}

TEST_CASE("Position.MakeMove.Castling Rights", "[Position][MakeMove]") {
    Position<Bitboard> position{FenString{"r3k2r/8/8/8/8/2B3n1/8/R3K2R w KQkq - 0 1"}};

    Move m1{
        .from = Square::C3,
        .to = Square::H8,
        .piece = Piece::WhiteBishop,
        .captured = Piece::BlackRook,
        .castling_rights_before{position.castling_rights()},
        .halfmove_clock_before = position.halfmove_clock(),
        .en_passant_target_before = position.en_passant_target(),
    };

    position.make_move(m1);
    CHECK(position.castling_rights()['K']);
    CHECK(position.castling_rights()['Q']);
    CHECK_FALSE(position.castling_rights()['k']);
    CHECK(position.castling_rights()['q']);

    Move m2{
        .from = Square::G3,
        .to = Square::H1,
        .piece = Piece::BlackKnight,
        .captured = Piece::WhiteRook,
        .castling_rights_before{position.castling_rights()},
        .halfmove_clock_before = position.halfmove_clock(),
        .en_passant_target_before = position.en_passant_target(),
    };

    position.make_move(m2);
    CHECK_FALSE(position.castling_rights()['K']);
    CHECK(position.castling_rights()['Q']);
    CHECK_FALSE(position.castling_rights()['k']);
    CHECK(position.castling_rights()['q']);

    Move m3{
        .from = Square::A1,
        .to = Square::A8,
        .piece = Piece::WhiteRook,
        .captured = Piece::BlackRook,
        .castling_rights_before{position.castling_rights()},
        .halfmove_clock_before = position.halfmove_clock(),
        .en_passant_target_before = position.en_passant_target(),
    };

    position.make_move(m3);
    CHECK_FALSE(position.castling_rights()['K']);
    CHECK_FALSE(position.castling_rights()['Q']);
    CHECK_FALSE(position.castling_rights()['k']);
    CHECK_FALSE(position.castling_rights()['q']);
}
