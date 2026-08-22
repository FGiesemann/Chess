/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/piece.h"

using namespace chesscore;

TEST_CASE("Core.Data.Piece.Type from Index", "[Piece]") {
    CHECK(piece_type_from_index(0) == PieceType::Pawn);
    CHECK(piece_type_from_index(1) == PieceType::Knight);
    CHECK(piece_type_from_index(2) == PieceType::Bishop);
    CHECK(piece_type_from_index(3) == PieceType::Rook);
    CHECK(piece_type_from_index(4) == PieceType::Queen);
    CHECK(piece_type_from_index(5) == PieceType::King);
    CHECK(piece_type_from_index(6) == PieceType::None);
}

TEST_CASE("Core.Data.Piece.Type from Char", "[Piece]") {
    CHECK(piece_type_from_char('r') == PieceType::Rook);
    CHECK(piece_type_from_char('n') == PieceType::Knight);
    CHECK(piece_type_from_char('b') == PieceType::Bishop);
    CHECK(piece_type_from_char('q') == PieceType::Queen);
    CHECK(piece_type_from_char('k') == PieceType::King);
    CHECK(piece_type_from_char('p') == PieceType::Pawn);

    CHECK(piece_type_from_char('R') == PieceType::Rook);
    CHECK(piece_type_from_char('N') == PieceType::Knight);
    CHECK(piece_type_from_char('B') == PieceType::Bishop);
    CHECK(piece_type_from_char('Q') == PieceType::Queen);
    CHECK(piece_type_from_char('K') == PieceType::King);
    CHECK(piece_type_from_char('P') == PieceType::Pawn);
    CHECK(piece_type_from_char('a') == PieceType::None);
}

TEST_CASE("Core.Data.Piece.Letter", "[Piece]") {
    CHECK(Piece::WhitePawn.piece_char() == 'P');
    CHECK(Piece::WhiteRook.piece_char() == 'R');
    CHECK(Piece::WhiteKnight.piece_char() == 'N');
    CHECK(Piece::WhiteBishop.piece_char() == 'B');
    CHECK(Piece::WhiteQueen.piece_char() == 'Q');
    CHECK(Piece::WhiteKing.piece_char() == 'K');
    CHECK(Piece::BlackPawn.piece_char() == 'p');
    CHECK(Piece::BlackRook.piece_char() == 'r');
    CHECK(Piece::BlackKnight.piece_char() == 'n');
    CHECK(Piece::BlackBishop.piece_char() == 'b');
    CHECK(Piece::BlackQueen.piece_char() == 'q');
    CHECK(Piece::BlackKing.piece_char() == 'k');
}

TEST_CASE("Core.Data.Piece.Letter colorless", "[Piece]") {
    CHECK(Piece::WhitePawn.piece_char_colorless() == 'P');
    CHECK(Piece::WhiteRook.piece_char_colorless() == 'R');
    CHECK(Piece::WhiteKnight.piece_char_colorless() == 'N');
    CHECK(Piece::WhiteBishop.piece_char_colorless() == 'B');
    CHECK(Piece::WhiteQueen.piece_char_colorless() == 'Q');
    CHECK(Piece::WhiteKing.piece_char_colorless() == 'K');
    CHECK(Piece::BlackPawn.piece_char_colorless() == 'P');
    CHECK(Piece::BlackRook.piece_char_colorless() == 'R');
    CHECK(Piece::BlackKnight.piece_char_colorless() == 'N');
    CHECK(Piece::BlackBishop.piece_char_colorless() == 'B');
    CHECK(Piece::BlackQueen.piece_char_colorless() == 'Q');
    CHECK(Piece::BlackKing.piece_char_colorless() == 'K');
}

TEST_CASE("Core.Data.Piece.Piece from Fen", "[Piece]") {
    CHECK(piece_from_fen_letter('P') == Piece::WhitePawn);
    CHECK(piece_from_fen_letter('R') == Piece::WhiteRook);
    CHECK(piece_from_fen_letter('N') == Piece::WhiteKnight);
    CHECK(piece_from_fen_letter('B') == Piece::WhiteBishop);
    CHECK(piece_from_fen_letter('Q') == Piece::WhiteQueen);
    CHECK(piece_from_fen_letter('K') == Piece::WhiteKing);
    CHECK(piece_from_fen_letter('p') == Piece::BlackPawn);
    CHECK(piece_from_fen_letter('r') == Piece::BlackRook);
    CHECK(piece_from_fen_letter('n') == Piece::BlackKnight);
    CHECK(piece_from_fen_letter('b') == Piece::BlackBishop);
    CHECK(piece_from_fen_letter('q') == Piece::BlackQueen);
    CHECK(piece_from_fen_letter('k') == Piece::BlackKing);
}
