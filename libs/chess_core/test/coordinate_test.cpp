/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/board.h"

using namespace chesscore;

TEST_CASE("Coordinates Checks") {
    SECTION("File from character (lower case)") {
        CHECK(File('a').file == 1);
        CHECK(File('b').file == 2);
        CHECK(File('c').file == 3);
        CHECK(File('d').file == 4);
        CHECK(File('e').file == 5);
        CHECK(File('f').file == 6);
        CHECK(File('g').file == 7);
        CHECK(File('h').file == 8);

        CHECK_THROWS_AS(File('i'), OutOfRange);
    }

    SECTION("File from character (upper case)") {
        CHECK(File('A').file == 1);
        CHECK(File('B').file == 2);
        CHECK(File('C').file == 3);
        CHECK(File('D').file == 4);
        CHECK(File('E').file == 5);
        CHECK(File('F').file == 6);
        CHECK(File('G').file == 7);
        CHECK(File('H').file == 8);

        CHECK_THROWS_AS(File('I'), OutOfRange);
    }

    SECTION("File from number") {
        CHECK(File(1).file == 1);
        CHECK(File(2).file == 2);
        CHECK(File(3).file == 3);
        CHECK(File(4).file == 4);
        CHECK(File(5).file == 5);
        CHECK(File(6).file == 6);
        CHECK(File(7).file == 7);
        CHECK(File(8).file == 8);

        CHECK_THROWS_AS(File(0), OutOfRange);
        CHECK_THROWS_AS(File(9), OutOfRange);
    }

    SECTION("File name") {
        CHECK(File(1).name() == 'a');
        CHECK(File(2).name() == 'b');
        CHECK(File(3).name() == 'c');
        CHECK(File(4).name() == 'd');
        CHECK(File(5).name() == 'e');
        CHECK(File(6).name() == 'f');
        CHECK(File(7).name() == 'g');
        CHECK(File(8).name() == 'h');
    }

    SECTION("Rank from number") {
        CHECK(Rank(1).rank == 1);
        CHECK(Rank(2).rank == 2);
        CHECK(Rank(3).rank == 3);
        CHECK(Rank(4).rank == 4);
        CHECK(Rank(5).rank == 5);
        CHECK(Rank(6).rank == 6);
        CHECK(Rank(7).rank == 7);
        CHECK(Rank(8).rank == 8);

        CHECK_THROWS_AS(Rank(0), OutOfRange);
        CHECK_THROWS_AS(Rank(9), OutOfRange);
    }
}
