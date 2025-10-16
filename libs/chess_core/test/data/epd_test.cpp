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

TEST_CASE("Data.EPD.Parse.Operations.Best Move", "[EPD]") {
    const auto record1 = parse_epd_line("8/3r4/pr1Pk1p1/8/7P/6P1/3R3K/5R2 w - - bm Re2+;");
    const auto &position1 = record1.position;

    CHECK(position1.piece_placement() == placement_from_string("_____R_____R___K______P________P________pr_Pk_p____r____________"));
    CHECK(position1.side_to_move() == Color::White);
    CHECK(position1.castling_rights() == CastlingRights::none());
    CHECK_FALSE(position1.en_passant_target().has_value());
    CHECK(position1.halfmove_clock() == 0);
    CHECK(position1.fullmove_number() == 1);
    CHECK(record1.bm == EpdRecord::move_list{"Re2+"});

    const auto record2 = parse_epd_line("r1b2rk1/2q1b1pp/p2ppn2/1p6/3QP3/1BN1B3/PPP3PP/R4RK1 w - - bm Nd5 a4;");
    const auto position2 = record2.position;

    CHECK(position2.piece_placement() == placement_from_string("R____RK_PPP___PP_BN_B______QP____p______p__ppn____q_b_ppr_b__rk_"));
    CHECK(position2.side_to_move() == Color::White);
    CHECK(position1.castling_rights() == CastlingRights::none());
    CHECK_FALSE(position2.en_passant_target().has_value());
    CHECK(position2.halfmove_clock() == 0);
    CHECK(position2.fullmove_number() == 1);
    CHECK(record2.bm == EpdRecord::move_list{"Nd5", "a4"});
}

TEST_CASE("Data.EPD.Parse.Operations.Multiple", "[EPD]") {
    const auto record =
        parse_epd_line(R"(r3k2r/pp4p1/2p1pnp1/3p1pn1/2PP4/1P2qPPP/P1Q3BK/3RBR2 b kq - bm Nxh3; id "arasan2024.200"; c0 "BOT ArasanX - BOT MateAI, lichess.org 2023";)");

    CHECK(record.bm == EpdRecord::move_list{"Nxh3"});
    CHECK(record.id == "arasan2024.200");
    REQUIRE(record.c.size() == 10);
    CHECK(record.c[0].has_value());
    CHECK(record.c[0] == "BOT ArasanX - BOT MateAI, lichess.org 2023");
}

TEST_CASE("Data.EPD.Parse.Operations.Operands", "[EPD]") {
    const auto record = parse_epd_line(
        R"(7b/8/kq6/8/8/1N2R3/K2P4/8 w - - bm a4; id "name"; c0 "comment0"; c1 "comment1"; acd 3; noop opA opB 123 "test operand" xyz; draw_reject; tcri player@server.de "Player Name";)"
    );

    CHECK(record.bm == EpdRecord::move_list{"a4"});
    CHECK(record.id == "name");
    CHECK(record.c[0].has_value());
    CHECK(record.c[0] == "comment0");
    CHECK(record.c[1].has_value());
    CHECK(record.c[1] == "comment1");
    CHECK(record.acd == 3);
    CHECK(record.noop_ops.size() == 5);
    CHECK(record.noop_ops == EpdRecord::str_list{"opA", "opB", "123", "test operand", "xyz"});
    CHECK(record.draw_reject);
    CHECK(record.tcri.first == "player@server.de");
    CHECK(record.tcri.second == "Player Name");
}

TEST_CASE("Data.EPD.Parse.Operations.Unknown Operation", "[EPD]") {
    const auto record = parse_epd_line(R"(7b/8/kq6/8/8/1N2R3/K2P4/8 w - - bm a4; D 123; id "test";)");

    CHECK(record.bm == EpdRecord::move_list{"a4"});
    CHECK(record.id == "test");
    CHECK(record.unknown_commands.size() == 1);
    CHECK(record.unknown_commands[0].opcode == "D");
    CHECK(record.unknown_commands[0].operands == std::vector<std::string>{"123"});
}

TEST_CASE("Data.EPD.File", "[EPD]") {
    const std::string epd_data = R"(r1b2rk1/2q1b1pp/p2ppn2/1p6/3QP3/1BN1B3/PPP3PP/R4RK1 w - - bm Nd5 a4;
r2qr1k1/1b1pppbp/1p4p1/pP2P1B1/3N4/R7/1PP2PPP/3QR1K1 w - a6
rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -
# Another one
7b/8/kq6/8/8/1N2R3/K2P4/8 w - - bm a4; id "test";)";

    std::istringstream epd_file{epd_data};
    const auto records = read_epd(epd_file);

    CHECK(records.size() == 4);
    CHECK(records[0].bm == EpdRecord::move_list{"Nd5", "a4"});
    CHECK(records[1].position.en_passant_target() == Square::A6);
    CHECK(records[2].position.castling_rights() == CastlingRights::all());
    CHECK(records[3].id == "test");
}
