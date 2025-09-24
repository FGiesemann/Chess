/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/bitboard.h"
#include "chesscore/perft.h"

using namespace chesscore;

TEST_CASE("Position.Perft.Initial Pos", "[Position][Perft]") {
    Position position{FenString::starting_position()};

    CHECK(perft(position, 0) == 1);
    CHECK(perft(position, 1) == 20);
    CHECK(perft(position, 2) == 400);
    CHECK(perft(position, 3) == 8902);
    CHECK(perft(position, 4) == 197281);
    // CHECK(perft(position, 5) == 4865609);
    // CHECK(perft(position, 6) == 119060324);
    // CHECK(perft(position, 7) == 3195901860);
    // CHECK(perft(position, 8) == 84998978956);
    // CHECK(perft(position, 9) == 2439530234167);
    // CHECK(perft(position, 10) == 69352859712417);
    // CHECK(perft(position, 11) == 2097651003696806);
}

TEST_CASE("Position.Perft.Kiwipete", "[Position][Perft]") {
    Position position{FenString{"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"}};

    CHECK(perft(position, 1) == 48);
    CHECK(perft(position, 2) == 2039);
    CHECK(perft(position, 3) == 97862);
    CHECK(perft(position, 4) == 4085603);
    // CHECK(perft(position, 5) == 193690690);
    // CHECK(perft(position, 6) == 8031647685);
}

TEST_CASE("Position.Perft.Position 3", "[Position][Perft]") {
    Position position{FenString{"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"}};

    CHECK(perft(position, 1) == 14);
    CHECK(perft(position, 2) == 191);
    CHECK(perft(position, 3) == 2812);
    CHECK(perft(position, 4) == 43238);
    CHECK(perft(position, 5) == 674624);
    // CHECK(perft(position, 6) == 11030083);
    // CHECK(perft(position, 7) == 178633661);
    // CHECK(perft(position, 8) == 3009794393);
}

TEST_CASE("Position.Perft.Position 5", "[Position][Perft]") {
    Position position{FenString{"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"}};

    CHECK(perft(position, 1) == 44);
    CHECK(perft(position, 2) == 1486);
    CHECK(perft(position, 3) == 62379);
    CHECK(perft(position, 4) == 2103487);
    // CHECK(perft(position, 5) == 89941194);
}

TEST_CASE("Position.Perft.Promotions", "[Position][Perft]") {
    Position position{FenString{"n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1"}};

    CHECK(perft(position, 1) == 24);
    CHECK(perft(position, 2) == 496);
    CHECK(perft(position, 3) == 9483);
    CHECK(perft(position, 4) == 182838);
    CHECK(perft(position, 5) == 3605103);
    // CHECK(perft(position, 6) == 71179139);
}
