/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/bitboard.h"

using namespace chesscore;

TEST_CASE("Bitboard.Bitmap.Empty", "[Bitmap][Basic]") {
    Bitmap bitmap{};
    CHECK(bitmap.empty());
    CHECK_FALSE(bitmap.get(Square::C5));
}

TEST_CASE("Bitboard.Bitmap.Init", "[Bitmap][Basic]") {
    Bitmap bitmap{Square::C5};
    CHECK_FALSE(bitmap.empty());
    CHECK(bitmap.get(Square::C5));
    CHECK_FALSE(bitmap.get(Square::C4));

    Bitmap bitmap2{0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000001ULL};
    CHECK_FALSE(bitmap2.empty());
    CHECK(bitmap2.get(Square::A1));

    bitmap2 = Bitmap{0b00000000'00000000'00000000'00001000'10000000'00000000'10000000'00000001ULL};
    CHECK_FALSE(bitmap2.empty());
    CHECK(bitmap2.get(Square::A1));
    CHECK(bitmap2.get(Square::H2));
    CHECK(bitmap2.get(Square::H4));
    CHECK(bitmap2.get(Square::D5));
}

TEST_CASE("Bitboard.Bitmap.Set, clear", "[Bitmap][Basic]") {
    Bitmap bitmap{};
    bitmap.set(Square::C5);
    CHECK_FALSE(bitmap.empty());
    CHECK(bitmap.get(Square::C5));
    CHECK_FALSE(bitmap.get(Square::C4));
    bitmap.clear(Square::C5);
    CHECK(bitmap.empty());
    CHECK_FALSE(bitmap.get(Square::C5));
}

TEST_CASE("Bitboard.Bitmap.Toggle", "[Bitmap][Basic]") {
    Bitmap bitmap{};
    bitmap.toggle(Square::C5);
    CHECK_FALSE(bitmap.empty());
    CHECK(bitmap.get(Square::C5));
    bitmap.toggle(Square::C5);
    CHECK(bitmap.empty());
    CHECK_FALSE(bitmap.get(Square::C5));
}

TEST_CASE("Bitboard.Bitmap.Multiple suqares", "[Bitmap][Basic]") {
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

TEST_CASE("Bitboard.Bitmap.Comparison", "[Bitmap][Operators]") {
    Bitmap bitmap1{};
    Bitmap bitmap2{};

    CHECK(bitmap1 == bitmap2);
    bitmap1.set(Square::C5);
    CHECK_FALSE(bitmap1 == bitmap2);
    bitmap2.toggle(Square::C5);
    CHECK(bitmap1 == bitmap2);
    bitmap1.clear(Square::C5);
    CHECK_FALSE(bitmap1 == bitmap2);
    bitmap2.clear(Square::C5);
    CHECK(bitmap1 == bitmap2);
}

TEST_CASE("Bitboard.Bitmap.Bitwise operators", "[Bitmap][Operators]") {
    Bitmap bitmap1{0x10'02'00'00'40'00'00'00ULL};
    Bitmap bitmap2{0x08'00'04'00'41'00'80'00ULL};

    auto bitmap3 = bitmap1 & bitmap2;
    CHECK(bitmap3.bits() == 0x00'00'00'00'40'00'00'00ULL);
    bitmap3 = bitmap1 | bitmap2;
    CHECK(bitmap3.bits() == 0x18'02'04'00'41'00'80'00ULL);
    bitmap3 = bitmap1 ^ bitmap2;
    CHECK(bitmap3.bits() == 0x18'02'04'00'01'00'80'00ULL);
    bitmap3 = ~bitmap1;
    CHECK(bitmap3.bits() == 0xEF'FD'FF'FF'BF'FF'FF'FFULL);
}
