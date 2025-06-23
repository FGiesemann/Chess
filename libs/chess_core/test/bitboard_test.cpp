/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/bitboard.h"

using namespace chesscore;

TEST_CASE("Empty bitmap", "[Bitmap][Basic]") {
    Bitmap bitmap{};
    CHECK(bitmap.empty());
    CHECK_FALSE(bitmap.get(Square::C5));
}

TEST_CASE("Bitmap Set, clear", "[Bitmap][Basic]") {
    Bitmap bitmap{};
    bitmap.set(Square::C5);
    CHECK_FALSE(bitmap.empty());
    CHECK(bitmap.get(Square::C5));
    CHECK_FALSE(bitmap.get(Square::C4));
    bitmap.clear(Square::C5);
    CHECK(bitmap.empty());
    CHECK_FALSE(bitmap.get(Square::C5));
}

TEST_CASE("Bitmap Toggle", "[Bitmap][Basic]") {
    Bitmap bitmap{};
    bitmap.toggle(Square::C5);
    CHECK_FALSE(bitmap.empty());
    CHECK(bitmap.get(Square::C5));
    bitmap.toggle(Square::C5);
    CHECK(bitmap.empty());
    CHECK_FALSE(bitmap.get(Square::C5));
}

TEST_CASE("Multiple suqares", "[Bitmap][Basic]") {
    Bitmap bitmap{};
    bitmap.set(Square::C5);
    bitmap.set(Square::D4);
    bitmap.set(Square::E3);
    bitmap.set(Square::F2);
    bitmap.set(Square::G1);
    CHECK_FALSE(bitmap.empty());
    CHECK(bitmap.get(Square::C5));
    CHECK(bitmap.get(Square::D4));
    CHECK(bitmap.get(Square::E3));
    CHECK(bitmap.get(Square::F2));
    CHECK(bitmap.get(Square::G1));
    CHECK_FALSE(bitmap.get(Square::H8));
    CHECK_FALSE(bitmap.get(Square::A1));
    bitmap.clear(Square::C5);
    CHECK_FALSE(bitmap.get(Square::C5));
    CHECK(bitmap.get(Square::D4));
    CHECK(bitmap.get(Square::E3));
    CHECK(bitmap.get(Square::F2));
    CHECK(bitmap.get(Square::G1));
    CHECK_FALSE(bitmap.get(Square::H8));
    CHECK_FALSE(bitmap.get(Square::A1));
}
