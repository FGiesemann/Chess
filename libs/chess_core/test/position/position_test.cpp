/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/bitboard.h"
#include "chesscore/position.h"

using namespace chesscore;

TEST_CASE("Position.Bitboard.Init.Empty", "[Position][Init]") {
    Position<Bitboard> position{};

    CHECK(position.board().empty());
    CHECK(position.side_to_move() == Color::White);
    CHECK(position.castling_rights() == CastlingRights::none());
    CHECK_FALSE(position.en_passant_target().has_value());
    CHECK(position.fullmove_number() == 1);
    CHECK(position.halfmove_clock() == 0);
}

TEST_CASE("Position.Bitboard.Init.FEN", "[Position][Init][FEN]") {
    Position<Bitboard> position1{FenString{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"}};
    CHECK_FALSE(position1.board().empty());
    CHECK(position1.side_to_move() == Color::White);
    CHECK(position1.castling_rights() == CastlingRights::all());
    CHECK_FALSE(position1.en_passant_target().has_value());
    CHECK(position1.fullmove_number() == 1);
    CHECK(position1.halfmove_clock() == 0);
    CHECK(position1.board().get_piece(Square::A1) == Piece::WhiteRook);
    CHECK(position1.board().get_piece(Square::B1) == Piece::WhiteKnight);
    CHECK(position1.board().get_piece(Square::C1) == Piece::WhiteBishop);
    CHECK(position1.board().get_piece(Square::D1) == Piece::WhiteQueen);
    CHECK(position1.board().get_piece(Square::E1) == Piece::WhiteKing);
    CHECK(position1.board().get_piece(Square::F1) == Piece::WhiteBishop);
    CHECK(position1.board().get_piece(Square::G1) == Piece::WhiteKnight);
    CHECK(position1.board().get_piece(Square::H1) == Piece::WhiteRook);
    CHECK(position1.board().get_piece(Square::A2) == Piece::WhitePawn);
    CHECK(position1.board().get_piece(Square::B2) == Piece::WhitePawn);
    CHECK(position1.board().get_piece(Square::C2) == Piece::WhitePawn);
    CHECK(position1.board().get_piece(Square::D2) == Piece::WhitePawn);
    CHECK(position1.board().get_piece(Square::E2) == Piece::WhitePawn);
    CHECK(position1.board().get_piece(Square::F2) == Piece::WhitePawn);
    CHECK(position1.board().get_piece(Square::G2) == Piece::WhitePawn);
    CHECK(position1.board().get_piece(Square::H2) == Piece::WhitePawn);
    CHECK(position1.board().get_piece(Square::A7) == Piece::BlackPawn);
    CHECK(position1.board().get_piece(Square::B7) == Piece::BlackPawn);
    CHECK(position1.board().get_piece(Square::C7) == Piece::BlackPawn);
    CHECK(position1.board().get_piece(Square::D7) == Piece::BlackPawn);
    CHECK(position1.board().get_piece(Square::E7) == Piece::BlackPawn);
    CHECK(position1.board().get_piece(Square::F7) == Piece::BlackPawn);
    CHECK(position1.board().get_piece(Square::G7) == Piece::BlackPawn);
    CHECK(position1.board().get_piece(Square::H7) == Piece::BlackPawn);
    CHECK(position1.board().get_piece(Square::A8) == Piece::BlackRook);
    CHECK(position1.board().get_piece(Square::B8) == Piece::BlackKnight);
    CHECK(position1.board().get_piece(Square::C8) == Piece::BlackBishop);
    CHECK(position1.board().get_piece(Square::D8) == Piece::BlackQueen);
    CHECK(position1.board().get_piece(Square::E8) == Piece::BlackKing);
    CHECK(position1.board().get_piece(Square::F8) == Piece::BlackBishop);
    CHECK(position1.board().get_piece(Square::G8) == Piece::BlackKnight);
    CHECK(position1.board().get_piece(Square::H8) == Piece::BlackRook);

    Position<Bitboard> position2{FenString{"r1bq1knr/p1pp1ppp/1pnb4/4p1B1/2BPP2P/2NQ2P1/PPP2P2/R3K1NR b KQ h3 0 1"}};
    CHECK_FALSE(position2.board().empty());
    CHECK(position2.side_to_move() == Color::Black);
    CHECK(position2.castling_rights()['K']);
    CHECK(position2.castling_rights()['Q']);
    CHECK_FALSE(position2.castling_rights()['k']);
    CHECK_FALSE(position2.castling_rights()['q']);
    CHECK(position2.en_passant_target().value() == Square::H3);
    CHECK(position2.fullmove_number() == 1);
    CHECK(position2.halfmove_clock() == 0);
    CHECK(position2.board().get_piece(Square::A1) == Piece::WhiteRook);
    CHECK_FALSE(position2.board().get_piece(Square::B1).has_value());
    CHECK(position2.board().get_piece(Square::E1) == Piece::WhiteKing);
    CHECK_FALSE(position2.board().get_piece(Square::F1).has_value());
    CHECK(position2.board().get_piece(Square::G1) == Piece::WhiteKnight);
}
