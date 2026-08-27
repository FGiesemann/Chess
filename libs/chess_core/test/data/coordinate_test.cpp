/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chess_core/square.h"

using namespace chess_core;

TEST_CASE("Core.Data.Coords.Coordinates Checks", "[Square]") {
    SECTION("File from character (lower case)") {
        CHECK(File('a').file == 0);
        CHECK(File('b').file == 1);
        CHECK(File('c').file == 2);
        CHECK(File('d').file == 3);
        CHECK(File('e').file == 4);
        CHECK(File('f').file == 5);
        CHECK(File('g').file == 6);
        CHECK(File('h').file == 7);
    }

    SECTION("File from character (upper case)") {
        CHECK(File('A').file == 0);
        CHECK(File('B').file == 1);
        CHECK(File('C').file == 2);
        CHECK(File('D').file == 3);
        CHECK(File('E').file == 4);
        CHECK(File('F').file == 5);
        CHECK(File('G').file == 6);
        CHECK(File('H').file == 7);
    }

    SECTION("File from number") {
        CHECK(File(0).file == 0);
        CHECK(File(1).file == 1);
        CHECK(File(2).file == 2);
        CHECK(File(3).file == 3);
        CHECK(File(4).file == 4);
        CHECK(File(5).file == 5);
        CHECK(File(6).file == 6);
        CHECK(File(7).file == 7);
    }

    SECTION("File name") {
        CHECK(File(0).name() == 'a');
        CHECK(File(1).name() == 'b');
        CHECK(File(2).name() == 'c');
        CHECK(File(3).name() == 'd');
        CHECK(File(4).name() == 'e');
        CHECK(File(5).name() == 'f');
        CHECK(File(6).name() == 'g');
        CHECK(File(7).name() == 'h');
    }

    SECTION("Rank from number") {
        CHECK(Rank(0).rank == 0);
        CHECK(Rank(1).rank == 1);
        CHECK(Rank(2).rank == 2);
        CHECK(Rank(3).rank == 3);
        CHECK(Rank(4).rank == 4);
        CHECK(Rank(5).rank == 5);
        CHECK(Rank(6).rank == 6);
        CHECK(Rank(7).rank == 7);
    }

    SECTION("Comparison operators") {
        CHECK(File(1) == File(1));
        CHECK(File(1) != File(2));
        CHECK(Rank(1) == Rank(1));
        CHECK(Rank(2) != Rank(1));
    }

    SECTION("Square comparisons") {
        CHECK(Square(File(1), Rank(1)) == Square(File(1), Rank(1)));
        CHECK(Square(File(1), Rank(1)) != Square(File(2), Rank(1)));
        CHECK(Square(File(1), Rank(1)) != Square(File(1), Rank(2)));
        CHECK(Square(File(1), Rank(1)) != Square(File(2), Rank(2)));
    }
}

TEST_CASE("Core.Data.Coords.Increments", "[Square]") {
    File file{'a'};
    Rank rank{0};

    Square square = Square::A1;

    file += 0;
    CHECK(file.file == 0);
    file += 2;
    CHECK(file.file == 2);
    file += 1;
    CHECK(file.file == 3);
    file += 7;
    CHECK(file.file == 2);

    rank += 0;
    CHECK(rank.rank == 0);
    rank += 2;
    CHECK(rank.rank == 2);
    rank += 1;
    CHECK(rank.rank == 3);
    rank += 4;
    CHECK(rank.rank == 7);
    rank += 3;
    CHECK(rank.rank == 2);

    square += 0;
    CHECK(square.file().file == 0);
    CHECK(square.rank().rank == 0);
    CHECK(square.index() == 0);
    square += 1;
    CHECK(square.file().file == 1);
    CHECK(square.rank().rank == 0);
    CHECK(square.index() == 1);
    square += 7;
    CHECK(square.file().file == 0);
    CHECK(square.rank().rank == 1);
    CHECK(square.index() == 8);
    square += 19;
    CHECK(square.file().file == 3);
    CHECK(square.rank().rank == 3);
    CHECK(square.index() == 27);
    square += 22;
    CHECK(square.file().file == 1);
    CHECK(square.rank().rank == 6);
    CHECK(square.index() == 49);
    square += 3;
    CHECK(square.file().file == 4);
    CHECK(square.rank().rank == 6);
    CHECK(square.index() == 52);
    square += 15;
    CHECK_FALSE(square.valid());
}

TEST_CASE("Core.Data.Coords.Decrements", "[Square]") {
    CHECK_FALSE((Square::A1 - 3).valid());
    CHECK(Square::E5 - 2 == Square::C5);
    CHECK(Square::F4 - 11 == Square::C3);
    CHECK(Square::E6 - 22 == Square::G3);
}

TEST_CASE("Core.Data.Coords.Defined Squares", "[Square]") {
    CHECK(Square::A1.file().file == 0);
    CHECK(Square::A1.rank().rank == 0);
    CHECK(Square::A1.file().name() == 'a');

    CHECK(Square::A8.file().file == 0);
    CHECK(Square::A8.rank().rank == 7);

    CHECK(Square::B1.file().file == 1);
    CHECK(Square::B1.rank().rank == 0);
    CHECK(Square::B8.file().file == 1);
    CHECK(Square::B8.rank().rank == 7);

    CHECK(Square::H1.file().file == 7);
    CHECK(Square::H1.rank().rank == 0);
    CHECK(Square::H8.file().file == 7);
    CHECK(Square::H8.rank().rank == 7);

    CHECK_FALSE(Square::None.valid());
}

TEST_CASE("Core.Data.Coords.Square index", "[Square]") {
    CHECK(Square::A1.index() == 0);
    CHECK(Square::B1.index() == 1);
    CHECK(Square::H1.index() == 7);
    CHECK(Square::A2.index() == 8);
    CHECK(Square::H2.index() == 15);
    CHECK(Square::A8.index() == 56);
    CHECK(Square::H8.index() == 63);
}

TEST_CASE("Core.Data.Coords.Square mirroring", "[Square]") {
    CHECK(Square::A1.mirrored() == Square{File{'a'}, Rank{7}});
    CHECK(Square::C2.mirrored() == Square{File{'c'}, Rank{6}});
    CHECK(Square::G3.mirrored() == Square{File{'g'}, Rank{5}});
    CHECK(Square::E4.mirrored() == Square{File{'e'}, Rank{4}});
    CHECK(Square::D5.mirrored() == Square{File{'d'}, Rank{3}});
    CHECK(Square::H6.mirrored() == Square{File{'h'}, Rank{2}});
    CHECK(Square::D7.mirrored() == Square{File{'d'}, Rank{1}});
    CHECK(Square::B8.mirrored() == Square{File{'b'}, Rank{0}});
}
