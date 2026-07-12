/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/magic.h"

using namespace chesscore;

TEST_CASE("Magic.Blocker.Iteration.Single Bit", "[magic]") {
    const Bitmap mask{0x0000000000000001ULL};

    Bitmap blockers;
    blockers = next_blocker_config(blockers, mask);
    CHECK(blockers == Bitmap{0x0000000000000001ULL});

    blockers = next_blocker_config(blockers, mask);
    CHECK(blockers.empty());
}

TEST_CASE("Magic.Blocker.Iteration.Consecutive Bits", "[magic]") {
    const Bitmap mask{0x0000000000000003ULL};

    Bitmap blockers;
    blockers = next_blocker_config(blockers, mask);
    CHECK(blockers == Bitmap{0x0000000000000001ULL});

    blockers = next_blocker_config(blockers, mask);
    CHECK(blockers == Bitmap{0x0000000000000002ULL});

    blockers = next_blocker_config(blockers, mask);
    CHECK(blockers == Bitmap{0x0000000000000003ULL});

    blockers = next_blocker_config(blockers, mask);
    CHECK(blockers.empty());
}

TEST_CASE("Magic.Blocker.Iteration.Spread Bits", "[magic]") {
    const Bitmap mask{0x000000000000002EULL};

    const std::array<Bitmap, 16> expected = {Bitmap{0x02ULL}, Bitmap{0x04ULL}, Bitmap{0x06ULL}, Bitmap{0x08ULL}, Bitmap{0x0AULL}, Bitmap{0x0CULL},
                                             Bitmap{0x0EULL}, Bitmap{0x20ULL}, Bitmap{0x22ULL}, Bitmap{0x24ULL}, Bitmap{0x26ULL}, Bitmap{0x28ULL},
                                             Bitmap{0x2AULL}, Bitmap{0x2CULL}, Bitmap{0x2EULL}, Bitmap{0x00ULL}};

    Bitmap blockers;
    for (const auto &expected_blocker : expected) {
        blockers = next_blocker_config(blockers, mask);
        CHECK(blockers == expected_blocker);
    }
}

TEST_CASE("Magic.Blocker.Iteration.Sign Bit", "[magic]") {
    const Bitmap mask{0xF000000000000000ULL};

    const std::array<Bitmap, 16> expected = {Bitmap{0x1000000000000000ULL}, Bitmap{0x2000000000000000ULL}, Bitmap{0x3000000000000000ULL}, Bitmap{0x4000000000000000ULL},
                                             Bitmap{0x5000000000000000ULL}, Bitmap{0x6000000000000000ULL}, Bitmap{0x7000000000000000ULL}, Bitmap{0x8000000000000000ULL},
                                             Bitmap{0x9000000000000000ULL}, Bitmap{0xA000000000000000ULL}, Bitmap{0xB000000000000000ULL}, Bitmap{0xC000000000000000ULL},
                                             Bitmap{0xD000000000000000ULL}, Bitmap{0xE000000000000000ULL}, Bitmap{0xF000000000000000ULL}, Bitmap{0x0000000000000000ULL}};

    Bitmap blockers;
    for (const auto &expected_blocker : expected) {
        blockers = next_blocker_config(blockers, mask);
        CHECK(blockers == expected_blocker);
    };
}
