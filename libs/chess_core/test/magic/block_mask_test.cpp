/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/magic.h"

using namespace chesscore;

TEST_CASE("Magic.BlockerMask.Non Sliding", "[magic]") {
    CHECK(blocker_mask(PieceType::King, Square::A5).empty());
}

TEST_CASE("Magic.BlockerMask.Rook", "[magic]") {
    const auto mask1 = blocker_mask(PieceType::Rook, Square::E3);
    CHECK(mask1 == Bitmap{0x00101010106E1000ULL});

    const auto mask2 = blocker_mask(PieceType::Rook, Square::H5);
    CHECK(mask2 == Bitmap{0x0080807E80808000ULL});
}

TEST_CASE("Magic.BlockerMask.Bishop", "[magic]") {
    const auto mask1 = blocker_mask(PieceType::Bishop, Square::E3);
    CHECK(mask1 == Bitmap{0x0000024428002800ULL});

    const auto mask2 = blocker_mask(PieceType::Bishop, Square::G8);
    CHECK(mask2 == Bitmap{0x0020100804020000ULL});

    const auto mask3 = blocker_mask(PieceType::Bishop, Square::C6);
    CHECK(mask3 == Bitmap{0x000A000A10204000ULL});
}

TEST_CASE("Magic.BlockerMask.Queen", "[magic]") {
    const auto mask1 = blocker_mask(PieceType::Queen, Square::E3);
    CHECK(mask1 == Bitmap{0x00101254386E3800ULL});

    const auto mask2 = blocker_mask(PieceType::Queen, Square::A6);
    CHECK(mask2 == Bitmap{0x00037E0305091100ULL});

    const auto mask3 = blocker_mask(PieceType::Queen, Square::H8);
    CHECK(mask3 == Bitmap{0x7EC0A09088848200ULL});
}
