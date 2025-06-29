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

    const auto white_moves{board.all_legal_moves(
        {.side_to_move = Color::White, .fullmove_number = 1, .halfmove_clock = 0, .castling_rights = CastlingRights::all(), .en_passant_target = std::nullopt}
    )};
    CHECK(white_moves.size() == 20);

    const auto black_moves{board.all_legal_moves(
        {.side_to_move = Color::Black, .fullmove_number = 1, .halfmove_clock = 0, .castling_rights = CastlingRights::all(), .en_passant_target = std::nullopt}
    )};
    CHECK(black_moves.size() == 20);
}
