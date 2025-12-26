/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/position.h"

using namespace chesscore;

TEST_CASE("Position.Hashing.Initialization", "[position][zobrist]") {
    const auto empty_position = Position{};
    const auto starting_position = Position::start_position();
    CHECK(empty_position.hash() == ZobristHash{});
    CHECK(starting_position.hash() == ZobristHash::starting_position_hash());
}

TEST_CASE("Position.Hashing.MakeMove.Simple", "[position][zobrist]") {
    auto position = Position::start_position();
    auto hash = ZobristHash::starting_position_hash();

    position.make_move(Move{.from = Square::E2, .to = Square::E3, .piece = Piece::WhitePawn});
    hash.move_piece(Piece::WhitePawn, Square::E2, Square::E3);
    hash.swap_side();
    CHECK(position.hash() == hash);

    position.make_move(Move{.from = Square::B8, .to = Square::C6, .piece = Piece::BlackKnight});
    hash.move_piece(Piece::BlackKnight, Square::B8, Square::C6);
    hash.swap_side();
    CHECK(position.hash() == hash);
}

TEST_CASE("Position.Hashing.MakeMove.En Passant", "[position][zobrist]") {
    auto position = Position::start_position();
    auto hash = ZobristHash::starting_position_hash();

    position.make_move(Move{.from = Square::E2, .to = Square::E4, .piece = Piece::WhitePawn});
    hash.move_piece(Piece::WhitePawn, Square::E2, Square::E4);
    hash.swap_side();
    hash.set_enpassant(File{'e'});
    CHECK(position.hash() == hash);
}

TEST_CASE("Position.Hashing.MakeMove.Capture", "[position][zobrist]") {
    CHECK(false); // TODO
}
