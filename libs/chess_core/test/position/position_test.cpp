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

TEST_CASE("Position.Bitboard.King in Check", "[Position]") {
    Position<Bitboard> position{FenString{"8/1pk5/8/Q7/8/5n2/6P1/4K3 w - - 0 1"}};

    CHECK(position.is_king_in_check(Color::White));
    CHECK(position.is_king_in_check(Color::Black));

    position.make_move(Move{.from = Square::G2, .to = Square::F3, .piece = Piece::WhitePawn, .captured = Piece::BlackKnight});
    CHECK_FALSE(position.is_king_in_check(Color::White));
    CHECK(position.is_king_in_check(Color::Black));

    position.make_move(Move{.from = Square::B7, .to = Square::B6, .piece = Piece::BlackPawn});
    CHECK_FALSE(position.is_king_in_check(Color::White));
    CHECK_FALSE(position.is_king_in_check(Color::Black));
}

TEST_CASE("Position.Bitboard.Check State.No Check", "[Position]") {
    CHECK(Position<Bitboard>{FenString::starting_position()}.check_state() == CheckState::None);
    CHECK(Position<Bitboard>{FenString{"6k1/5pbp/6p1/8/1R6/4B2P/r4PP1/6K1 w - - 0 1"}}.check_state() == CheckState::None);
    CHECK(Position<Bitboard>{FenString{"r1bq1b1r/pppp1Qpp/2n2n2/4p3/2B1P3/8/PPPP1PPP/RNB1K1NR w KQkq - 0 1"}}.check_state() == CheckState::None);
}

TEST_CASE("Position.Bitboard.Check State.Check", "[Position]") {
    CHECK(Position<Bitboard>{FenString{"2k5/1pp4p/p1n5/6r1/8/2B5/PP3P1P/5RK1 w - - 0 1"}}.check_state() == CheckState::Check);
    CHECK(Position<Bitboard>{FenString{"2rR2k1/pp3ppp/4b3/6B1/8/7P/PP3PP1/6K1 b - - 0 1"}}.check_state() == CheckState::Check);
    CHECK(Position<Bitboard>{FenString{"5rk1/pp4pp/8/8/4bP2/1N4P1/PP5P/3R3K w - - 0 1"}}.check_state() == CheckState::Check);
    CHECK(Position<Bitboard>{FenString{"8/6pk/6p1/2pB2b1/2P5/1P4PQ/6K1/q7 b - - 0 1"}}.check_state() == CheckState::Check);
}

TEST_CASE("Position.Bitboard.Check State.Checkmate", "[Position]") {
    CHECK(Position<Bitboard>{FenString{"4k3/4Q3/4K3/8/8/8/8/8 b - - 0 1"}}.check_state() == CheckState::Checkmate);
    CHECK(Position<Bitboard>{FenString{"7k/8/7K/3BB3/8/8/8/8 b - - 0 1"}}.check_state() == CheckState::Checkmate);
    CHECK(Position<Bitboard>{FenString{"4rbk1/pb1n1ppQ/1pq4p/2p5/8/2NB1N2/PPP2PPP/5RK1 b - - 0 1"}}.check_state() == CheckState::Checkmate);
    CHECK(Position<Bitboard>{FenString{"r1bqkb1r/pppp1Qpp/2n2n2/4p3/2B1P3/8/PPPP1PPP/RNB1K1NR w KQkq - 0 1"}}.check_state() == CheckState::None);
}

TEST_CASE("Position.Bitboard.Check State.Stalemate", "[Position]") {
    CHECK(Position<Bitboard>{FenString{"k7/8/1KN5/8/8/8/8/8 b - - 0 1"}}.check_state() == CheckState::Stalemate);
    CHECK(Position<Bitboard>{FenString{"8/8/8/8/8/4q1k1/8/7K w - - 0 1"}}.check_state() == CheckState::Stalemate);
    CHECK(Position<Bitboard>{FenString{"8/pkp5/1p4q1/3b4/8/3n4/2r5/5K2 w - - 0 1"}}.check_state() == CheckState::Stalemate);
    CHECK(Position<Bitboard>{FenString{"8/8/8/8/8/5k1q/8/6K1 w - - 0 1"}}.check_state() == CheckState::Stalemate);
    CHECK(Position<Bitboard>{FenString{"7k/8/6Q1/8/2K5/8/8/8 b - - 0 1"}}.check_state() == CheckState::Stalemate);
}
