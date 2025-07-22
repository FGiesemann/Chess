/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/bitboard.h"
#include "chesscore/fen.h"
#include "chesscore/move.h"
#include "chesscore/position.h"

using namespace chesscore;

TEST_CASE("Bitboard.Bitboard.MoveGeneration.Knight (no check)", "[Bitboard][MoveGeneration]") {
    Position<Bitboard> position1{FenString{"2k1n3/6N1/8/1P1b1r2/3P4/2N5/4q3/KP6 w - - 0 1"}};
    MoveList moves1{};
    position1.board().all_knight_moves(moves1, position1.state());
    CHECK(moves1.size() == 10);
    CHECK(move_list_contains(moves1, Move{Square::C3, Square::A4, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves1, Move{Square::C3, Square::D5, Piece::WhiteKnight, Piece::BlackBishop}));
    CHECK(move_list_contains(moves1, Move{Square::C3, Square::E4, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves1, Move{Square::C3, Square::E2, Piece::WhiteKnight, Piece::BlackQueen}));
    CHECK(move_list_contains(moves1, Move{Square::C3, Square::D1, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves1, Move{Square::C3, Square::A2, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves1, Move{Square::G7, Square::E8, Piece::WhiteKnight, Piece::BlackKnight}));
    CHECK(move_list_contains(moves1, Move{Square::G7, Square::H5, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves1, Move{Square::G7, Square::F5, Piece::WhiteKnight, Piece::BlackRook}));
    CHECK(move_list_contains(moves1, Move{Square::G7, Square::E6, Piece::WhiteKnight}));

    Position<Bitboard> position2{FenString{"8/2p5/1P3q2/3n4/5P1r/2N1B3/6n1/8 b - - 0 1"}};
    MoveList moves2{};
    position2.board().all_knight_moves(moves2, position2.state());
    CHECK(moves2.size() == 9);
    CHECK(move_list_contains(moves2, Move{Square::D5, Square::E7, Piece::BlackKnight}));
    CHECK(move_list_contains(moves2, Move{Square::D5, Square::F4, Piece::BlackKnight, Piece::WhitePawn}));
    CHECK(move_list_contains(moves2, Move{Square::D5, Square::E3, Piece::BlackKnight, Piece::WhiteBishop}));
    CHECK(move_list_contains(moves2, Move{Square::D5, Square::C3, Piece::BlackKnight, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves2, Move{Square::D5, Square::B4, Piece::BlackKnight}));
    CHECK(move_list_contains(moves2, Move{Square::D5, Square::B6, Piece::BlackKnight, Piece::WhitePawn}));
    CHECK(move_list_contains(moves2, Move{Square::G2, Square::F4, Piece::BlackKnight, Piece::WhitePawn}));
    CHECK(move_list_contains(moves2, Move{Square::G2, Square::E1, Piece::BlackKnight}));
    CHECK(move_list_contains(moves2, Move{Square::G2, Square::E3, Piece::BlackKnight, Piece::WhiteBishop}));
}

TEST_CASE("Bitboard.Bitboard.MoveGeneration.King (no check)", "[Bitboard][MoveGeneration]") {
    Position<Bitboard> position1{FenString{"8/1b1P4/2k5/8/5P2/5K2/4p3/8 w - - 0 1"}};
    MoveList moves1{};
    position1.board().all_king_moves(moves1, position1.state());
    CHECK(moves1.size() == 7);
    CHECK(move_list_contains(moves1, Move{Square::F3, Square::E4, Piece::WhiteKing}));
    CHECK(move_list_contains(moves1, Move{Square::F3, Square::G4, Piece::WhiteKing}));
    CHECK(move_list_contains(moves1, Move{Square::F3, Square::G3, Piece::WhiteKing}));
    CHECK(move_list_contains(moves1, Move{Square::F3, Square::G2, Piece::WhiteKing}));
    CHECK(move_list_contains(moves1, Move{Square::F3, Square::F2, Piece::WhiteKing}));
    CHECK(move_list_contains(moves1, Move{Square::F3, Square::E2, Piece::WhiteKing, Piece::BlackPawn}));
    CHECK(move_list_contains(moves1, Move{Square::F3, Square::E3, Piece::WhiteKing}));

    Position<Bitboard> position2{FenString{"8/1b1P4/2k5/8/5P2/5K2/4p3/8 b - - 0 1"}};
    MoveList moves2{};
    position2.board().all_king_moves(moves2, position2.state());
    CHECK(moves2.size() == 7);
    CHECK(move_list_contains(moves2, Move{Square::C6, Square::C7, Piece::BlackKing}));
    CHECK(move_list_contains(moves2, Move{Square::C6, Square::D7, Piece::BlackKing, Piece::WhitePawn}));
    CHECK(move_list_contains(moves2, Move{Square::C6, Square::D6, Piece::BlackKing}));
    CHECK(move_list_contains(moves2, Move{Square::C6, Square::D5, Piece::BlackKing}));
    CHECK(move_list_contains(moves2, Move{Square::C6, Square::C5, Piece::BlackKing}));
    CHECK(move_list_contains(moves2, Move{Square::C6, Square::B5, Piece::BlackKing}));
    CHECK(move_list_contains(moves2, Move{Square::C6, Square::B6, Piece::BlackKing}));
}

TEST_CASE("Bitboard.Bitboard.MoveGeneration.King Castling.No Check", "[Bitboard][MoveGeneration]") {
    Position<Bitboard> position{FenString{"8/8/8/8/8/8/8/R3K2R w KQkq - 0 1"}};
    MoveList moves{};
    position.board().all_king_moves(moves, position.state());
    CHECK(moves.size() == 7);
    CHECK(move_list_contains(moves, Move{Square::E1, Square::C1, Piece::WhiteKing}));
    CHECK(move_list_contains(moves, Move{Square::E1, Square::D1, Piece::WhiteKing}));
    CHECK(move_list_contains(moves, Move{Square::E1, Square::D2, Piece::WhiteKing}));
    CHECK(move_list_contains(moves, Move{Square::E1, Square::E2, Piece::WhiteKing}));
    CHECK(move_list_contains(moves, Move{Square::E1, Square::F2, Piece::WhiteKing}));
    CHECK(move_list_contains(moves, Move{Square::E1, Square::F1, Piece::WhiteKing}));
    CHECK(move_list_contains(moves, Move{Square::E1, Square::G1, Piece::WhiteKing}));
}

TEST_CASE("Bitboard.Bitboard.MoveGeneration.King Castling.King in Check", "[Bitboard][MoveGeneration]") {
    Position<Bitboard> position{FenString{"4r3/8/8/8/8/8/8/R3K2R w KQkq - 0 1"}};
    MoveList moves{};
    position.board().all_king_moves(moves, position.state());
    CHECK(moves.size() == 4);
    CHECK(move_list_contains(moves, Move{Square::E1, Square::D1, Piece::WhiteKing}));
    CHECK(move_list_contains(moves, Move{Square::E1, Square::D2, Piece::WhiteKing}));
    CHECK(move_list_contains(moves, Move{Square::E1, Square::F1, Piece::WhiteKing}));
    CHECK(move_list_contains(moves, Move{Square::E1, Square::F2, Piece::WhiteKing}));
}

TEST_CASE("Bitboard.Bitboard.MoveGeneration.King Castling.Trough Check", "[Bitboard][MoveGeneration]") {
    Position<Bitboard> position{FenString{"r3k2r/8/8/8/7B/8/8/8 b KQkq - 0 1"}};
    MoveList moves{};
    position.board().all_king_moves(moves, position.state());
    CHECK(moves.size() == 4);
    CHECK(move_list_contains(moves, Move{Square::E8, Square::D7, Piece::BlackKing}));
    CHECK(move_list_contains(moves, Move{Square::E8, Square::F7, Piece::BlackKing}));
    CHECK(move_list_contains(moves, Move{Square::E8, Square::F8, Piece::BlackKing}));
    CHECK(move_list_contains(moves, Move{Square::E8, Square::G8, Piece::BlackKing}));
}

TEST_CASE("Bitboard.Bitboard.MoveGeneration.King Castling.Not Allowed", "[Bitboard][MoveGeneration]") {
    Position<Bitboard> position{FenString{"r3k2r/8/8/8/7B/8/8/8 b KQq - 0 1"}};
    MoveList moves{};
    position.board().all_king_moves(moves, position.state());
    CHECK(moves.size() == 3);
    CHECK(move_list_contains(moves, Move{Square::E8, Square::D7, Piece::BlackKing}));
    CHECK(move_list_contains(moves, Move{Square::E8, Square::F7, Piece::BlackKing}));
    CHECK(move_list_contains(moves, Move{Square::E8, Square::F8, Piece::BlackKing}));
}

TEST_CASE("Bitboard.Bitboard.MoveGeneration.SlidingPieces.Ray N", "[Bitboard][MoveGeneration]") {
    Position<Bitboard> position1{FenString{"8/5b2/2p1P2P/8/1pP2Rp1/6p1/3b4/8 w - - 0 1"}};
    MoveList moves1{};
    position1.board().all_moves_along_ray(Piece::WhiteRook, Square::F4, RayDirection::North, moves1, position1.state());
    CHECK(moves1.size() == 3);
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::F5, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::F6, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::F7, Piece::WhiteRook, Piece::BlackBishop}));
}

TEST_CASE("Bitboard.Bitboard.MoveGeneration.SlidingPieces.Ray W", "[Bitboard][MoveGeneration]") {
    Position<Bitboard> position1{FenString{"8/5b2/2p1P2P/8/1pP2Rp1/6p1/3b4/8 w - - 0 1"}};
    MoveList moves1{};
    position1.board().all_moves_along_ray(Piece::WhiteRook, Square::F4, RayDirection::West, moves1, position1.state());
    CHECK(moves1.size() == 2);
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::E4, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::D4, Piece::WhiteRook}));
}

TEST_CASE("Bitboard.Bitboard.MoveGeneration.SlidingPieces.Rook (no check)", "[Bitboard][MoveGeneration]") {
    Position<Bitboard> position1{FenString{"8/5b2/2p1P2P/8/1pP2Rp1/6p1/3b4/8 w - - 0 1"}};
    MoveList moves1{};
    position1.board().all_sliding_moves(Piece::WhiteRook, Square::F4, moves1, position1.state());
    CHECK(moves1.size() == 9);
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::F1, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::F2, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::F3, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::G4, Piece::WhiteRook, Piece::BlackPawn}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::F5, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::F6, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::F7, Piece::WhiteRook, Piece::BlackBishop}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::E4, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::F4, Square::D4, Piece::WhiteRook}));
}

TEST_CASE("Bitboard.Bitboard.MoveGeneration.SlidingPieces.Bishop (no check)", "[Bitboard][MoveGeneration]") {
    Position<Bitboard> position1{FenString{"8/5b2/2p1P2P/8/1pP2Rp1/6p1/3b4/8 b - - 0 1"}};
    MoveList moves1{};
    position1.board().all_sliding_moves(Piece::BlackBishop, Square::D2, moves1, position1.state());
    CHECK(moves1.size() == 5);
    CHECK(move_list_contains(moves1, Move{Square::D2, Square::C3, Piece::BlackBishop}));
    CHECK(move_list_contains(moves1, Move{Square::D2, Square::C1, Piece::BlackBishop}));
    CHECK(move_list_contains(moves1, Move{Square::D2, Square::E1, Piece::BlackBishop}));
    CHECK(move_list_contains(moves1, Move{Square::D2, Square::E3, Piece::BlackBishop}));
    CHECK(move_list_contains(moves1, Move{Square::D2, Square::F4, Piece::BlackBishop, Piece::WhiteRook}));
}

TEST_CASE("Bitboard.Bitboard.MoveGeneration.SlidingPieces.Queen (no check)", "[Bitboard][MoveGeneration]") {
    Position<Bitboard> position1{FenString{"1q4pr/2P3p1/1np1brNR/8/2Q2Pp1/3p4/2P1P1bn/8 w - - 0 1"}};
    MoveList moves1{};
    position1.board().all_sliding_moves(Piece::WhiteQueen, Square::C4, moves1, position1.state());
    CHECK(moves1.size() == 14);
    CHECK(move_list_contains(moves1, Move{Square::C4, Square::A2, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::C4, Square::B3, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::C4, Square::D5, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::C4, Square::E6, Piece::WhiteQueen, Piece::BlackBishop}));
    CHECK(move_list_contains(moves1, Move{Square::C4, Square::A6, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::C4, Square::B5, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::C4, Square::D3, Piece::WhiteQueen, Piece::BlackPawn}));
    CHECK(move_list_contains(moves1, Move{Square::C4, Square::A4, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::C4, Square::B4, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::C4, Square::D4, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::C4, Square::E4, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::C4, Square::C3, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::C4, Square::C5, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::C4, Square::C6, Piece::WhiteQueen, Piece::BlackPawn}));
}

TEST_CASE("Bitboard.Bitboard.MoveGeneration.Pawn.Single Step", "[Bitboard][MoveGeneration]") {
    Position<Bitboard> position1{FenString{"8/3p2p1/1p1p4/2p3Pp/P3Pp1P/2PP2p1/P2P4/8 w - - 0 1"}};
    MoveList moves1;
    position1.board().all_pawn_moves(moves1, position1.state());
    CHECK(moves1.size() == 6);
    CHECK(move_list_contains(moves1, Move{Square::A2, Square::A3, Piece::WhitePawn}));
    CHECK(move_list_contains(moves1, Move{Square::A4, Square::A5, Piece::WhitePawn}));
    CHECK(move_list_contains(moves1, Move{Square::C3, Square::C4, Piece::WhitePawn}));
    CHECK(move_list_contains(moves1, Move{Square::D3, Square::D4, Piece::WhitePawn}));
    CHECK(move_list_contains(moves1, Move{Square::E4, Square::E5, Piece::WhitePawn}));
    CHECK(move_list_contains(moves1, Move{Square::G5, Square::G6, Piece::WhitePawn}));

    Position<Bitboard> position2{FenString{"8/3p2p1/1p1p4/2p3Pp/P3Pp1P/2PP2p1/P2P4/8 b - - 0 1"}};
    MoveList moves2;
    position2.board().all_pawn_moves(moves2, position2.state());
    CHECK(moves2.size() == 6);
    CHECK(move_list_contains(moves2, Move{Square::B6, Square::B5, Piece::BlackPawn}));
    CHECK(move_list_contains(moves2, Move{Square::C5, Square::C4, Piece::BlackPawn}));
    CHECK(move_list_contains(moves2, Move{Square::D6, Square::D5, Piece::BlackPawn}));
    CHECK(move_list_contains(moves2, Move{Square::F4, Square::F3, Piece::BlackPawn}));
    CHECK(move_list_contains(moves2, Move{Square::G7, Square::G6, Piece::BlackPawn}));
    CHECK(move_list_contains(moves2, Move{Square::G3, Square::G2, Piece::BlackPawn}));
}

TEST_CASE("Bitboard.Bitboard.MoveGeneration.Pawn.Double Step", "[Bitboard][MoveGeneration]") {
    Position<Bitboard> position1{FenString{"3b4/p1p1p1pp/3pn3/1p5r/1P5N/3B1P2/P2PP1PP/6Q1 w - - 0 1"}};
    MoveList moves1;
    position1.board().all_pawn_moves(moves1, position1.state());
    CHECK(moves1.size() == 8);
    CHECK(move_list_contains(moves1, Move{Square::A2, Square::A3, Piece::WhitePawn}));
    CHECK(move_list_contains(moves1, Move{Square::A2, Square::A4, Piece::WhitePawn}));
    CHECK(move_list_contains(moves1, Move{Square::E2, Square::E3, Piece::WhitePawn}));
    CHECK(move_list_contains(moves1, Move{Square::E2, Square::E4, Piece::WhitePawn}));
    CHECK(move_list_contains(moves1, Move{Square::F3, Square::F4, Piece::WhitePawn}));
    CHECK(move_list_contains(moves1, Move{Square::G2, Square::G3, Piece::WhitePawn}));
    CHECK(move_list_contains(moves1, Move{Square::G2, Square::G4, Piece::WhitePawn}));
    CHECK(move_list_contains(moves1, Move{Square::H2, Square::H3, Piece::WhitePawn}));

    Position<Bitboard> position2{FenString{"3b4/p1p1p1pp/3pn3/1p5r/1P5N/3B1P2/P2PP1PP/6Q1 b - - 0 1"}};
    MoveList moves2;
    position2.board().all_pawn_moves(moves2, position2.state());
    CHECK(moves2.size() == 8);
    CHECK(move_list_contains(moves2, Move{Square::A7, Square::A6, Piece::BlackPawn}));
    CHECK(move_list_contains(moves2, Move{Square::A7, Square::A5, Piece::BlackPawn}));
    CHECK(move_list_contains(moves2, Move{Square::C7, Square::C6, Piece::BlackPawn}));
    CHECK(move_list_contains(moves2, Move{Square::C7, Square::C5, Piece::BlackPawn}));
    CHECK(move_list_contains(moves2, Move{Square::D6, Square::D5, Piece::BlackPawn}));
    CHECK(move_list_contains(moves2, Move{Square::G7, Square::G6, Piece::BlackPawn}));
    CHECK(move_list_contains(moves2, Move{Square::G7, Square::G5, Piece::BlackPawn}));
    CHECK(move_list_contains(moves2, Move{Square::H7, Square::H6, Piece::BlackPawn}));
}

TEST_CASE("Bitboard.Bitboard.MoveGeneration.Pawn.Captures", "[Bitboard][MoveGeneration]") {
    Position<Bitboard> position1{FenString{"8/3n3p/1p1Pn2q/P3rPp1/1B1r2b1/b1P5/7P/8 w - g6 0 1"}};
    MoveList moves1;
    position1.board().all_pawn_moves(moves1, position1.state());
    CHECK(moves1.size() == 9);
    CHECK(move_list_contains(moves1, Move{Square::A5, Square::A6, Piece::WhitePawn}));
    CHECK(move_list_contains(moves1, Move{Square::A5, Square::B6, Piece::WhitePawn, Piece::BlackPawn}));
    CHECK(move_list_contains(moves1, Move{Square::C3, Square::C4, Piece::WhitePawn}));
    CHECK(move_list_contains(moves1, Move{Square::C3, Square::D4, Piece::WhitePawn, Piece::BlackRook}));
    CHECK(move_list_contains(moves1, Move{Square::F5, Square::E6, Piece::WhitePawn, Piece::BlackKnight}));
    CHECK(move_list_contains(moves1, Move{Square::F5, Square::F6, Piece::WhitePawn}));
    CHECK(move_list_contains(moves1, Move{Square::F5, Square::G6, Piece::WhitePawn, Piece::BlackPawn, true}));
    CHECK(move_list_contains(moves1, Move{Square::H2, Square::H3, Piece::WhitePawn}));
    CHECK(move_list_contains(moves1, Move{Square::H2, Square::H4, Piece::WhitePawn}));

    Position<Bitboard> position2{FenString{"4Q3/1p1ppp2/N1Bn1R1R/5B2/p4pPp/1N5P/8/8 b - g3 0 1"}};
    MoveList moves2;
    position2.board().all_pawn_moves(moves2, position2.state());
    CHECK(moves2.size() == 13);
    CHECK(move_list_contains(moves2, Move{Square::A4, Square::A3, Piece::BlackPawn}));
    CHECK(move_list_contains(moves2, Move{Square::A4, Square::B3, Piece::BlackPawn, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves2, Move{Square::B7, Square::A6, Piece::BlackPawn, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves2, Move{Square::B7, Square::B6, Piece::BlackPawn}));
    CHECK(move_list_contains(moves2, Move{Square::B7, Square::B5, Piece::BlackPawn}));
    CHECK(move_list_contains(moves2, Move{Square::B7, Square::C6, Piece::BlackPawn, Piece::WhiteBishop}));
    CHECK(move_list_contains(moves2, Move{Square::D7, Square::C6, Piece::BlackPawn, Piece::WhiteBishop}));
    CHECK(move_list_contains(moves2, Move{Square::E7, Square::E6, Piece::BlackPawn}));
    CHECK(move_list_contains(moves2, Move{Square::E7, Square::E5, Piece::BlackPawn}));
    CHECK(move_list_contains(moves2, Move{Square::E7, Square::F6, Piece::BlackPawn, Piece::WhiteRook}));
    CHECK(move_list_contains(moves2, Move{Square::F4, Square::F3, Piece::BlackPawn}));
    CHECK(move_list_contains(moves2, Move{Square::F4, Square::G3, Piece::BlackPawn, Piece::WhitePawn, true}));
    CHECK(move_list_contains(moves2, Move{Square::H4, Square::G3, Piece::BlackPawn, Piece::WhitePawn, true}));
}

TEST_CASE("Bitboard.Bitboard.MoveGeneration.Pawn.Promotions", "[Bitboard][MoveGeneration]") {
    Position<Bitboard> position1{FenString{"R2r1N2/PPP1P3/6P1/8/8/p7/3pp2p/2n2R2 w - - 0 1"}};
    MoveList moves1;
    position1.board().all_pawn_moves(moves1, position1.state());
    CHECK(moves1.size() == 21);
    CHECK(move_list_contains_promotions(moves1, Move{Square::B7, Square::B8, Piece::WhitePawn}));
    CHECK(move_list_contains_promotions(moves1, Move{Square::C7, Square::C8, Piece::WhitePawn}));
    CHECK(move_list_contains_promotions(moves1, Move{Square::C7, Square::D8, Piece::WhitePawn, Piece::BlackRook}));
    CHECK(move_list_contains_promotions(moves1, Move{Square::E7, Square::D8, Piece::WhitePawn, Piece::BlackRook}));
    CHECK(move_list_contains_promotions(moves1, Move{Square::E7, Square::E8, Piece::WhitePawn}));
    CHECK(move_list_contains(moves1, Move{Square::G6, Square::G7, Piece::WhitePawn}));

    Position<Bitboard> position2{FenString{"R2r1N2/PPP1P3/6P1/8/8/p7/3pp2p/2n2R2 b - - 0 1"}};
    MoveList moves2;
    position2.board().all_pawn_moves(moves2, position2.state());
    CHECK(moves2.size() == 17);
    CHECK(move_list_contains(moves2, Move{Square::A3, Square::A2, Piece::BlackPawn}));
    CHECK(move_list_contains_promotions(moves2, Move{Square::D2, Square::D1, Piece::BlackPawn}));
    CHECK(move_list_contains_promotions(moves2, Move{Square::E2, Square::E1, Piece::BlackPawn}));
    CHECK(move_list_contains_promotions(moves2, Move{Square::E2, Square::F1, Piece::BlackPawn, Piece::WhiteRook}));
    CHECK(move_list_contains_promotions(moves2, Move{Square::H2, Square::H1, Piece::BlackPawn}));
}

TEST_CASE("Bitboard.Bitboard.MoveGeneration.Check After Move", "[Bitboard][MoveGeneration]") {
    Position<Bitboard> position1{FenString{"2k2r2/3bp1q1/1bN5/8/8/4P1NQ/8/r2R2K1 w - - 0 1"}};
    MoveList moves1 = position1.board().all_legal_moves(position1.state());
    CHECK(moves1.size() == 29);

    CHECK(move_list_contains(moves1, Move{Square::C6, Square::B8, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves1, Move{Square::C6, Square::D8, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves1, Move{Square::C6, Square::A7, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves1, Move{Square::C6, Square::E7, Piece::WhiteKnight, Piece::BlackPawn}));
    CHECK(move_list_contains(moves1, Move{Square::C6, Square::E5, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves1, Move{Square::C6, Square::A5, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves1, Move{Square::C6, Square::B4, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves1, Move{Square::C6, Square::D4, Piece::WhiteKnight}));
    CHECK(move_list_contains(moves1, Move{Square::D1, Square::A1, Piece::WhiteRook, Piece::BlackRook}));
    CHECK(move_list_contains(moves1, Move{Square::D1, Square::B1, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::D1, Square::C1, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::D1, Square::E1, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::D1, Square::F1, Piece::WhiteRook}));
    CHECK(move_list_contains(moves1, Move{Square::G1, Square::G2, Piece::WhiteKing}));
    CHECK(move_list_contains(moves1, Move{Square::G1, Square::H2, Piece::WhiteKing}));
    CHECK(move_list_contains(moves1, Move{Square::G1, Square::H1, Piece::WhiteKing}));
    CHECK(move_list_contains(moves1, Move{Square::H3, Square::D7, Piece::WhiteQueen, Piece::BlackBishop}));
    CHECK(move_list_contains(moves1, Move{Square::H3, Square::E6, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::H3, Square::F5, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::H3, Square::G4, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::H3, Square::H2, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::H3, Square::H1, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::H3, Square::H4, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::H3, Square::H5, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::H3, Square::H6, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::H3, Square::H7, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::H3, Square::H8, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::H3, Square::G2, Piece::WhiteQueen}));
    CHECK(move_list_contains(moves1, Move{Square::H3, Square::F1, Piece::WhiteQueen}));

    CHECK_FALSE(move_list_contains(moves1, Move{Square::D1, Square::D2, Piece::WhiteRook}));
    CHECK_FALSE(move_list_contains(moves1, Move{Square::D1, Square::D3, Piece::WhiteRook}));
    CHECK_FALSE(move_list_contains(moves1, Move{Square::D1, Square::D4, Piece::WhiteRook}));
    CHECK_FALSE(move_list_contains(moves1, Move{Square::D1, Square::D5, Piece::WhiteRook}));
    CHECK_FALSE(move_list_contains(moves1, Move{Square::D1, Square::D6, Piece::WhiteRook}));
    CHECK_FALSE(move_list_contains(moves1, Move{Square::D1, Square::D7, Piece::WhiteRook, Piece::BlackBishop}));
    CHECK_FALSE(move_list_contains(moves1, Move{Square::E3, Square::E4, Piece::WhitePawn}));
    CHECK_FALSE(move_list_contains(moves1, Move{Square::G3, Square::F1, Piece::WhiteKnight}));
    CHECK_FALSE(move_list_contains(moves1, Move{Square::G3, Square::H1, Piece::WhiteKnight}));
    CHECK_FALSE(move_list_contains(moves1, Move{Square::G3, Square::E2, Piece::WhiteKnight}));
    CHECK_FALSE(move_list_contains(moves1, Move{Square::G3, Square::E4, Piece::WhiteKnight}));
    CHECK_FALSE(move_list_contains(moves1, Move{Square::G3, Square::F5, Piece::WhiteKnight}));
    CHECK_FALSE(move_list_contains(moves1, Move{Square::G3, Square::H5, Piece::WhiteKnight}));
    CHECK_FALSE(move_list_contains(moves1, Move{Square::G1, Square::F1, Piece::WhiteKing}));
    CHECK_FALSE(move_list_contains(moves1, Move{Square::G1, Square::F2, Piece::WhiteKing}));
}
