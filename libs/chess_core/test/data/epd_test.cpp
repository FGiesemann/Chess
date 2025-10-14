/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/epd.h"

using namespace chesscore;

TEST_CASE("Data.EPD.Parse.Positions.Starting", "[EPD]") {
    const auto record = parse_epd_line("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
    const auto &position = record.position;

    CHECK(position.piece_placement() == starting_piece_placement());
    CHECK(position.side_to_move() == Color::White);
    CHECK(position.castling_rights() == CastlingRights::all());
    CHECK_FALSE(position.en_passant_target().has_value());
    CHECK(position.halfmove_clock() == 0);
    CHECK(position.fullmove_number() == 1);
}

TEST_CASE("Data.EPD.Parse.Positions.Position 1", "[EPD]") {
    const auto record = parse_epd_line("8/6pk/p3p3/2Q1P3/1p2P3/5P1P/5KP1/1q6 w - -");
    const auto &position = record.position;

    CHECK(position.piece_placement() == placement_from_string("_q___________KP______P_P_p__P_____Q_P___p___p_________pk________"));
    CHECK(position.side_to_move() == Color::White);
    CHECK(position.castling_rights() == CastlingRights::none());
    CHECK_FALSE(position.en_passant_target().has_value());
    CHECK(position.halfmove_clock() == 0);
    CHECK(position.fullmove_number() == 1);
}

TEST_CASE("Data.EPD.Parse.Positions.Position 2", "[EPD]") {
    const auto record = parse_epd_line("r1bqk2r/pp2bppp/2p5/3pP3/P2Q1P2/2N1B3/1PP3PP/R4RK1 b kq -");
    const auto &position = record.position;

    CHECK(position.piece_placement() == placement_from_string("R____RK__PP___PP__N_B___P__Q_P_____pP_____p_____pp__bpppr_bqk__r"));
    CHECK(position.side_to_move() == Color::Black);
    CHECK(position.castling_rights().black_kingside);
    CHECK(position.castling_rights().black_queenside);
    CHECK_FALSE(position.en_passant_target().has_value());
    CHECK(position.halfmove_clock() == 0);
    CHECK(position.fullmove_number() == 1);
}

TEST_CASE("Data.EPD.Parse.Positions.Position 3", "[EPD]") {
    const auto record = parse_epd_line("r2qr1k1/1b1pppbp/1p4p1/pP2P1B1/3N4/R7/1PP2PPP/3QR1K1 w - a6");
    const auto &position = record.position;

    CHECK(position.piece_placement() == placement_from_string("___QR_K__PP__PPPR__________N____pP__P_B__p____p__b_pppbpr__qr_k_"));
    CHECK(position.side_to_move() == Color::White);
    CHECK(position.castling_rights() == CastlingRights::none());
    REQUIRE(position.en_passant_target().has_value());
    CHECK(position.en_passant_target().value() == Square::A6);
    CHECK(position.halfmove_clock() == 0);
    CHECK(position.fullmove_number() == 1);
}
