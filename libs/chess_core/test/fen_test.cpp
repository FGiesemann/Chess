/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/fen.h"

using namespace chesscore;

TEST_CASE("Empty FEN string", "[FENString][Init]") {
    CHECK(FenString{}.str() == "8/8/8/8/8/8/8/8 w - - 0 1");
}

TEST_CASE("Starting position FEN string", "[FENString][Init]") {
    CHECK(FenString::startingPosition().str() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

TEST_CASE("Piece placement", "[FENString][Validity]") {
    CHECK(detail::check_piece_placement("8/8/8/8/8/8/8/8 w - - 0 1") == 16);
    CHECK(detail::check_piece_placement("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") == 44);
    CHECK(detail::check_piece_placement("rnbqkbnr/ppppppp1/p7/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") == 45);
    CHECK(detail::check_piece_placement("rnbqkbnr/ppppppp1/6p1/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") == 46);

    // invalid piece types
    CHECK_THROWS_AS(detail::check_piece_placement("rnbqXbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"), InvalidFen);
    CHECK_THROWS_AS(detail::check_piece_placement("rnbqkbnr/pXpppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"), InvalidFen);
    CHECK_THROWS_AS(detail::check_piece_placement("rnbqkbnr/pppppppp/8/8/8/8/PPPPPXPP/RNBQKBNR w KQkq - 0 1"), InvalidFen);
    CHECK_THROWS_AS(detail::check_piece_placement("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNX w KQkq - 0 1"), InvalidFen);
    CHECK_THROWS_AS(detail::check_piece_placement("rnbqkbnr/Xppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"), InvalidFen);

    // invalid number of ranks
    CHECK_THROWS_AS(detail::check_piece_placement("rnbqkbnr/pppppppp/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"), InvalidFen);      // Too few
    CHECK_THROWS_AS(detail::check_piece_placement("rnbqkbnr/pppp4/4pppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"), InvalidFen); // Too many

    // invalid number of files
    CHECK_THROWS_AS(detail::check_piece_placement("rnbqkbnrQ/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"), InvalidFen); // Too many
    CHECK_THROWS_AS(detail::check_piece_placement("rnbqkbnr/ppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"), InvalidFen);   // Too few
    CHECK_THROWS_AS(detail::check_piece_placement("rnbqkbnr/pppppppp/8/6/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"), InvalidFen);  // Too few
    CHECK_THROWS_AS(detail::check_piece_placement("rnbqkbnr/pppppppp/8/9/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"), InvalidFen);  // Too many
    CHECK_THROWS_AS(detail::check_piece_placement("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNRp w KQkq - 0 1"), InvalidFen); // Too many
    CHECK_THROWS_AS(detail::check_piece_placement("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBN w KQkq - 0 1"), InvalidFen);   // Too few

    // invalid empty fields specification
    CHECK_THROWS_AS(detail::check_piece_placement("rnbqkbnr/pppppppp/8/44/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"), InvalidFen); // two adjecent numbers
    CHECK_THROWS_AS(detail::check_piece_placement("rnbqkbnr/pppppp11/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"), InvalidFen);  // two adjecent numbers
    CHECK_THROWS_AS(detail::check_piece_placement("rnbqkbnr/pp21ppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"), InvalidFen);   // two adjecent numbers
}

TEST_CASE("Side to move", "[FENString][Validity]") {
    CHECK(detail::check_side_to_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 44) == 46);
    CHECK(detail::check_side_to_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1", 44) == 46);
    CHECK(detail::check_side_to_move("8/8/8/8/8/8/8/8 w - - 0 1", 16) == 18);

    // Invalid side to move
    CHECK_THROWS_AS(detail::check_side_to_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR x KQkq - 0 1", 44), InvalidFen);  // invalid color
    CHECK_THROWS_AS(detail::check_side_to_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR ww KQkq - 0 1", 44), InvalidFen); // two letters

    // Invalid start pos
    CHECK_THROWS_AS(detail::check_side_to_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1", 34), InvalidFen);
}

TEST_CASE("Castling availability", "[FENString][Validity]") {
    CHECK(detail::check_castling_availability("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 46) == 51);
    CHECK(detail::check_castling_availability("8/8/8/8/8/8/8/8 w - - 0 1", 18) == 20);

    // Invalid castling specification
    CHECK_THROWS_AS(detail::check_castling_availability("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KPkq - 0 1", 45), InvalidFen);  // invalid piece
    CHECK_THROWS_AS(detail::check_castling_availability("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkqk - 0 1", 45), InvalidFen); // too many pieces
    CHECK_THROWS_AS(detail::check_castling_availability("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KkQq - 0 1", 45), InvalidFen);  // invalid order
}
