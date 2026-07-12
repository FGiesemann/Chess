/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/magic.h"

using namespace chesscore;

TEST_CASE("Magic.Attack Map.Rook", "[magic]") {
    const auto blockers1 = Bitmap{0x0004000022000000ULL};
    const auto attack1 = Bitmap{0x000404043A040404ULL};
    CHECK(attack_bitmap(PieceType::Rook, Square::C4, blockers1) == attack1);

    const auto blockers2 = Bitmap{0x000080800A000080ULL};
    const auto attack2 = Bitmap{0x0000008078808080ULL};
    CHECK(attack_bitmap(PieceType::Rook, Square::H4, blockers2) == attack2);

    const auto blockers3 = Bitmap{0x0200580000020200ULL};
    const auto attack3 = Bitmap{0x02020C0202020000ULL};
    CHECK(attack_bitmap(PieceType::Rook, Square::B6, blockers3) == attack3);
}

TEST_CASE("Magic.Attack Map.Bishop", "[magic]") {
    const auto blockers1 = Bitmap{0x0000200000000010ULL};
    const auto attack1 = Bitmap{0x000020110A000A11ULL};
    CHECK(attack_bitmap(PieceType::Bishop, Square::C3, blockers1) == attack1);

    const auto blockers2 = Bitmap{0x1000040000000408ULL};
    const auto attack2 = Bitmap{0x0000040200020400ULL};
    CHECK(attack_bitmap(PieceType::Bishop, Square::A4, blockers2) == attack2);

    const auto blockers3 = Bitmap{0x0204000008400200ULL};
    const auto attack3 = Bitmap{0x8044280028400000ULL};
    CHECK(attack_bitmap(PieceType::Bishop, Square::E5, blockers3) == attack3);
}
