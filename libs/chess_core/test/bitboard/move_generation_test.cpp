/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/bitboard.h"
#include "chesscore/fen.h"
#include "chesscore/move.h"

using namespace chesscore;

TEST_CASE("Bitboard.Bitboard.MoveGeneration.Starting", "[Bitboard][MoveGeneration]") {
    Bitboard board{FenString::starting_position()};

    const auto white_moves{board.all_legal_moves(Color::White, CastlingRights::all(), std::nullopt)};
    CHECK(white_moves.size() == 20);

    const auto black_moves{board.all_legal_moves(Color::Black, CastlingRights::all(), std::nullopt)};
    CHECK(black_moves.size() == 20);
}
