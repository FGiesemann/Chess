/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/position.h"

#include <ranges>

using namespace chesscore;

namespace {

auto find_move(const MoveList &moves, const Move &ref_move) -> Move {
    const auto item = std::ranges::find_if(moves, [&](const Move &m) -> bool { return is_moving_same_piece(m, ref_move); });
    if (item != moves.end()) {
        return *item;
    } else {
        throw ChessException("Could not find move");
    }
}

} // namespace

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
    auto position = Position{FenString{"1k2b3/2n3n1/r6R/7Q/1B1pP3/3B4/6N1/2K5 b - e3 0 1"}};
    auto hash = ZobristHash::from_position(position);

    position.make_move(Move{.from = Square::A6, .to = Square::H6, .piece = Piece::BlackRook, .captured = Piece::WhiteRook});
    hash.clear_piece(Piece::WhiteRook, Square::H6);
    hash.move_piece(Piece::BlackRook, Square::A6, Square::H6);
    hash.clear_enpassant(File{'e'});
    hash.swap_side();
    CHECK(position.hash() == hash);

    position.make_move(Move{.from = Square::H5, .to = Square::E8, .piece = Piece::WhiteQueen, .captured = Piece::BlackBishop});
    hash.clear_piece(Piece::BlackBishop, Square::E8);
    hash.move_piece(Piece::WhiteQueen, Square::H5, Square::E8);
    hash.swap_side();
    CHECK(position.hash() == hash);

    position.make_move(Move{.from = Square::C7, .to = Square::E8, .piece = Piece::BlackKnight, .captured = Piece::WhiteQueen});
    hash.clear_piece(Piece::WhiteQueen, Square::E8);
    hash.move_piece(Piece::BlackKnight, Square::C7, Square::E8);
    hash.swap_side();
    CHECK(position.hash() == hash);
}

TEST_CASE("Position.Hashing.MakeMove.En Passant Capture", "[position][zobrist]") {
    auto position = Position{FenString{"1k2b3/2n3n1/r6R/7Q/1B1pP3/3B4/6N1/2K5 b - e3 0 1"}};
    auto hash = ZobristHash::from_position(position);

    position.make_move(Move{.from = Square::D4, .to = Square::E3, .piece = Piece::BlackPawn, .captured = Piece::WhitePawn, .capturing_en_passant = true});
    hash.clear_piece(Piece::WhitePawn, Square::E4);
    hash.move_piece(Piece::BlackPawn, Square::D4, Square::E3);
    hash.clear_enpassant(File{'e'});
    hash.swap_side();
    CHECK(position.hash() == hash);
}

TEST_CASE("Position.Hashing.MakeMove.Castling", "[position][zobrist]") {
    const auto position_w = Position{FenString{"r3k2r/1pp2ppp/2n2n2/8/2B1PN2/2N2Q2/PPPP1PPP/R3K2R w KQkq - 0 1"}};
    const auto hash_w = ZobristHash::from_position(position_w);

    auto position2 = position_w;
    position2.make_move(Move{.from = Square::E1, .to = Square::C1, .piece = Piece::WhiteKing});
    auto hash2 = hash_w;
    hash2.move_piece(Piece::WhiteKing, Square::E1, Square::C1);
    hash2.move_piece(Piece::WhiteRook, Square::A1, Square::D1);
    hash2.switch_castling(CastlingRights::all(), CastlingRights{false, false, true, true});
    hash2.swap_side();
    CHECK(position2.hash() == hash2);

    auto position3 = position_w;
    position3.make_move(Move{.from = Square::E1, .to = Square::G1, .piece = Piece::WhiteKing});
    auto hash3 = hash_w;
    hash3.move_piece(Piece::WhiteKing, Square::E1, Square::G1);
    hash3.move_piece(Piece::WhiteRook, Square::H1, Square::F1);
    hash3.switch_castling(CastlingRights::all(), CastlingRights{false, false, true, true});
    hash3.swap_side();
    CHECK(position3.hash() == hash3);

    const auto position_b = Position{FenString{"r3k2r/1pp2ppp/2n2n2/8/2B1PN2/2N2Q2/PPPP1PPP/R3K2R b KQkq - 0 1"}};
    const auto hash_b = ZobristHash::from_position(position_b);

    auto position4 = position_b;
    position4.make_move(Move{.from = Square::E8, .to = Square::C8, .piece = Piece::BlackKing});
    auto hash4 = hash_b;
    hash4.move_piece(Piece::BlackKing, Square::E8, Square::C8);
    hash4.move_piece(Piece::BlackRook, Square::A8, Square::D8);
    hash4.switch_castling(CastlingRights::all(), CastlingRights{true, true, false, false});
    hash4.swap_side();
    CHECK(position4.hash() == hash4);

    auto position5 = position_b;
    position5.make_move(Move{.from = Square::E8, .to = Square::G8, .piece = Piece::BlackKing});
    auto hash5 = hash_b;
    hash5.move_piece(Piece::BlackKing, Square::E8, Square::G8);
    hash5.move_piece(Piece::BlackRook, Square::H8, Square::F8);
    hash5.switch_castling(CastlingRights::all(), CastlingRights{true, true, false, false});
    hash5.swap_side();
    CHECK(position5.hash() == hash5);
}

TEST_CASE("Position.Hashing.MakeMove.Promotion", "[position][zobrist]") {
    auto position_w = Position{FenString{"8/5P2/8/8/8/8/1p6/8 w - - 0 1"}};
    auto hash_w = ZobristHash::from_position(position_w);

    position_w.make_move(Move{.from = Square::F7, .to = Square::F8, .piece = Piece::WhitePawn, .promoted = Piece::WhiteQueen});
    hash_w.clear_piece(Piece::WhitePawn, Square::F7);
    hash_w.set_piece(Piece::WhiteQueen, Square::F8);
    hash_w.swap_side();
    CHECK(position_w.hash() == hash_w);

    auto position_b = Position{FenString{"8/5P2/8/8/8/8/1p6/8 b - - 0 1"}};
    auto hash_b = ZobristHash::from_position(position_b);

    position_b.make_move(Move{.from = Square::B2, .to = Square::B1, .piece = Piece::BlackPawn, .promoted = Piece::BlackBishop});
    hash_b.clear_piece(Piece::BlackPawn, Square::B2);
    hash_b.set_piece(Piece::BlackBishop, Square::B1);
    hash_b.swap_side();
    CHECK(position_b.hash() == hash_b);
}

TEST_CASE("Position.Hashing.UnmakeMove.Simple", "[position][zobrist]") {
    auto position = Position{FenString{"r1bqkbnr/pppppppp/2n5/8/8/4P3/PPPP1PPP/RNBQKBNR w KQkq - 0 1"}};
    auto hash = ZobristHash::from_position(position);
    const auto move1 = Move{.from = Square::B8, .to = Square::C6, .piece = Piece::BlackKnight, .castling_rights_before = CastlingRights::all()};
    const auto move2 = Move{.from = Square::E2, .to = Square::E3, .piece = Piece::WhitePawn, .castling_rights_before = CastlingRights::all()};

    position.unmake_move(move1);
    hash.move_piece(Piece::BlackKnight, Square::C6, Square::B8);
    hash.swap_side();
    CHECK(position.hash() == hash);

    position.unmake_move(move2);
    hash.move_piece(Piece::WhitePawn, Square::E3, Square::E2);
    hash.swap_side();
    CHECK(position.hash() == hash);
    CHECK(position.hash() == ZobristHash::starting_position_hash());
}

TEST_CASE("Position.Hashing.UnmakeMove.En Passant", "[position][zobrist]") {
    auto position = Position{FenString{"rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"}};
    auto hash = ZobristHash::from_position(position);

    position.unmake_move(Move{.from = Square::E2, .to = Square::E4, .piece = Piece::WhitePawn, .castling_rights_before = CastlingRights::all()});
    hash.move_piece(Piece::WhitePawn, Square::E4, Square::E2);
    hash.clear_enpassant(File{'e'});
    hash.swap_side();
    CHECK(position.hash() == hash);
    CHECK(position.hash() == ZobristHash::starting_position_hash());
}

TEST_CASE("Position.Hashing.UnmakeMove.Capture", "[position][zobrist]") {
    auto position = Position{FenString{"1k2b3/2n3n1/r6R/7Q/1B1pP3/3B4/6N1/2K5 b - e3 0 1"}};
    const auto start_hash = ZobristHash::from_position(position);
    const auto move1 = find_move(position.all_legal_moves(), Move{.from = Square::A6, .to = Square::H6, .piece = Piece::BlackRook, .captured = Piece::WhiteRook});
    position.make_move(move1);
    const auto move2 = find_move(position.all_legal_moves(), Move{.from = Square::H5, .to = Square::E8, .piece = Piece::WhiteQueen, .captured = Piece::BlackBishop});
    position.make_move(move2);
    const auto move3 = find_move(position.all_legal_moves(), Move{.from = Square::C7, .to = Square::E8, .piece = Piece::BlackKnight, .captured = Piece::WhiteQueen});
    position.make_move(move3);
    auto hash = ZobristHash::from_position(position);

    position.unmake_move(move3);
    hash.move_piece(Piece::BlackKnight, Square::E8, Square::C7);
    hash.set_piece(Piece::WhiteQueen, Square::E8);
    hash.swap_side();
    CHECK(position.hash() == hash);

    position.unmake_move(move2);
    hash.move_piece(Piece::WhiteQueen, Square::E8, Square::H5);
    hash.set_piece(Piece::BlackBishop, Square::E8);
    hash.swap_side();
    CHECK(position.hash() == hash);

    position.unmake_move(move1);
    hash.move_piece(Piece::BlackRook, Square::H6, Square::A6);
    hash.set_piece(Piece::WhiteRook, Square::H6);
    hash.set_enpassant(File{'e'});
    hash.swap_side();
    CHECK(position.hash() == hash);
    CHECK(position.hash() == start_hash);
}

TEST_CASE("Position.Hashing.UnmakeMove.En Passant Capture", "[position][zobrist]") {
    auto position = Position{FenString{"1k2b3/2n3n1/r6R/7Q/1B1pP3/3B4/6N1/2K5 b - e3 0 1"}};
    const auto start_hash = ZobristHash::from_position(position);
    const auto move =
        find_move(position.all_legal_moves(), Move{.from = Square::D4, .to = Square::E3, .piece = Piece::BlackPawn, .captured = Piece::WhitePawn, .capturing_en_passant = true});
    position.make_move(move);
    auto hash = ZobristHash::from_position(position);

    position.unmake_move(move);
    hash.move_piece(Piece::BlackPawn, Square::E3, Square::D4);
    hash.set_piece(Piece::WhitePawn, Square::E4);
    hash.set_enpassant(File{'e'});
    hash.swap_side();
    CHECK(position.hash() == hash);
    CHECK(position.hash() == start_hash);
}

TEST_CASE("Position.Hashing.UnmakeMove.Castling", "[position][zobrist]") {
    const auto position_w = Position{FenString{"r3k2r/1pp2ppp/2n2n2/8/2B1PN2/2N2Q2/PPPP1PPP/R3K2R w KQkq - 0 1"}};
    const auto start_hash_w = ZobristHash::from_position(position_w);

    auto position1 = position_w;
    const auto move1 = find_move(position1.all_legal_moves(), Move{.from = Square::E1, .to = Square::C1, .piece = Piece::WhiteKing});
    position1.make_move(move1);
    position1.unmake_move(move1);
    CHECK(position1.hash() == start_hash_w);

    auto position2 = position_w;
    const auto move2 = find_move(position2.all_legal_moves(), Move{.from = Square::E1, .to = Square::G1, .piece = Piece::WhiteKing});
    position2.make_move(move2);
    position2.unmake_move(move2);
    CHECK(position2.hash() == start_hash_w);

    const auto position_b = Position{FenString{"r3k2r/1pp2ppp/2n2n2/8/2B1PN2/2N2Q2/PPPP1PPP/R3K2R b KQkq - 0 1"}};
    const auto start_hash_b = ZobristHash::from_position(position_b);

    auto position3 = position_b;
    const auto move3 = find_move(position3.all_legal_moves(), Move{.from = Square::E8, .to = Square::C8, .piece = Piece::BlackKing});
    position3.make_move(move3);
    position3.unmake_move(move3);
    CHECK(position3.hash() == start_hash_b);

    auto position4 = position_b;
    const auto move4 = find_move(position4.all_legal_moves(), Move{.from = Square::E8, .to = Square::G8, .piece = Piece::BlackKing});
    position4.make_move(move4);
    position4.unmake_move(move4);
    CHECK(position4.hash() == start_hash_b);
}

TEST_CASE("Position.Hashing.UnmakeMove.Promotion", "[position][zobrist]") {
    auto position_w = Position{FenString{"8/5P2/8/8/8/8/1p6/8 w - - 0 1"}};
    auto hash_w = ZobristHash::from_position(position_w);
    const auto move_w = find_move(position_w.all_legal_moves(), Move{.from = Square::F7, .to = Square::F8, .piece = Piece::WhitePawn, .promoted = Piece::WhiteQueen});
    position_w.make_move(move_w);
    position_w.unmake_move(move_w);
    CHECK(position_w.hash() == hash_w);

    auto position_b = Position{FenString{"8/5P2/8/8/8/8/1p6/8 b - - 0 1"}};
    auto hash_b = ZobristHash::from_position(position_b);
    const auto move_b = find_move(position_b.all_legal_moves(), Move{.from = Square::B2, .to = Square::B1, .piece = Piece::BlackPawn, .promoted = Piece::BlackBishop});
    position_b.make_move(move_b);
    position_b.unmake_move(move_b);
    CHECK(position_b.hash() == hash_b);
}
