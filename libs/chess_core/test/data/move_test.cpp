/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chess_core/move.h"

using namespace chesscore;

TEST_CASE("Core.Data.Move.Comparison", "[Move][Compare]") {
    Move m1{
        .from = Square::E2,
        .to = Square::E4,
        .piece = Piece::WhitePawn,
        .captured{},
        .capturing_en_passant{false},
        .promoted{},
        .castling_rights_before{CastlingRights::all()},
        .halfmove_clock_before = 0,
        .en_passant_target_before{}
    };
    Move m1_p{
        .from = Square::E2,
        .to = Square::E4,
        .piece = Piece::BlackPawn,
        .captured{},
        .capturing_en_passant{false},
        .promoted{},
        .castling_rights_before{CastlingRights::all()},
        .halfmove_clock_before = 0,
        .en_passant_target_before{}
    };
    Move m2{
        .from = Square::E2,
        .to = Square::E4,
        .piece = Piece::WhitePawn,
        .captured{},
        .capturing_en_passant{false},
        .promoted{},
        .castling_rights_before{CastlingRights::all()},
        .halfmove_clock_before = 0,
        .en_passant_target_before{}
    };

    CHECK(is_moving_same_piece(m1, m2));

    m1.from = Square::A1;
    CHECK_FALSE(is_moving_same_piece(m1, m2));

    m1.from = Square::E2;
    m2.to = Square::F3;
    CHECK_FALSE(is_moving_same_piece(m1, m2));

    m2.to = Square::E4;
    CHECK_FALSE(is_moving_same_piece(m1_p, m2));

    m2.captured = Piece::BlackPawn;
    CHECK_FALSE(is_moving_same_piece(m1, m2));

    m1.captured = Piece::BlackPawn;
    m1.capturing_en_passant = true;
    CHECK_FALSE(is_moving_same_piece(m1, m2));

    m2.capturing_en_passant = true;
    CHECK(is_moving_same_piece(m1, m2));

    m1.halfmove_clock_before = 45;
    CHECK(is_moving_same_piece(m1, m2));

    m2.en_passant_target_before = Square::G2;
    CHECK(is_moving_same_piece(m1, m2));
}
