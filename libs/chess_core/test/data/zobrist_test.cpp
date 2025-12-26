/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/zobrist.h"

using namespace chesscore;

TEST_CASE("Data.Zobrist.ZobristKeys.Initialization", "[zobrist]") {
    ZobristKeys::initialize();
    CHECK(ZobristKeys::initialized());
}

TEST_CASE("Data.Zobrist.ZobristKeys.Nonzero", "[zobrist]") {
    ZobristKeys::initialize();

    CHECK(ZobristKeys::side_key() != 0);

    for (int rank = Rank::min_rank; rank <= Rank::max_rank; ++rank) {
        CHECK(ZobristKeys::enpassant_key(Rank{rank}) != 0);
    }

    CastlingRights rights{};
    CHECK(ZobristKeys::castling_key(rights) != 0);
    rights.black_kingside = true;
    CHECK(ZobristKeys::castling_key(rights) != 0); // k
    rights.black_kingside = false;
    rights.black_queenside = true;
    CHECK(ZobristKeys::castling_key(rights) != 0); // q
    rights.black_kingside = true;
    CHECK(ZobristKeys::castling_key(rights) != 0); // kq
    rights.black_kingside = false;
    rights.black_queenside = false;
    rights.white_kingside = true;
    CHECK(ZobristKeys::castling_key(rights) != 0); // K
    rights.black_kingside = true;
    CHECK(ZobristKeys::castling_key(rights) != 0); // Kk
    rights.black_kingside = false;
    rights.black_queenside = true;
    CHECK(ZobristKeys::castling_key(rights) != 0); // Kq
    rights.black_kingside = true;
    CHECK(ZobristKeys::castling_key(rights) != 0); // Kkq
    rights.black_kingside = false;
    rights.black_queenside = false;
    rights.white_kingside = false;
    rights.white_queenside = true;
    CHECK(ZobristKeys::castling_key(rights) != 0); // Q
    rights.black_kingside = true;
    CHECK(ZobristKeys::castling_key(rights) != 0); // Qk
    rights.black_kingside = false;
    rights.black_queenside = true;
    CHECK(ZobristKeys::castling_key(rights) != 0); // Qq
    rights.black_kingside = true;
    CHECK(ZobristKeys::castling_key(rights) != 0); // Qkq
    rights.black_kingside = false;
    rights.black_queenside = false;
    rights.white_kingside = true;
    CHECK(ZobristKeys::castling_key(rights) != 0); // QK
    rights.black_kingside = true;
    CHECK(ZobristKeys::castling_key(rights) != 0); // QKk
    rights.black_kingside = false;
    rights.black_queenside = true;
    CHECK(ZobristKeys::castling_key(rights) != 0); // QKq
    rights.black_kingside = true;
    CHECK(ZobristKeys::castling_key(rights) != 0); // QKkq

    auto color = Color::White;
    for (const auto &type : all_piece_types) {
        Square square{Square::A1};
        for (int square_index = 0; square_index < Square::count; ++square_index) {
            CHECK(ZobristKeys::piece_key(Piece{.type = type, .color = color}, square) != 0);
        }
    }
    color = Color::Black;
    for (const auto &type : all_piece_types) {
        Square square{Square::A1};
        for (int square_index = 0; square_index < Square::count; ++square_index) {
            CHECK(ZobristKeys::piece_key(Piece{.type = type, .color = color}, square) != 0);
        }
    }
}

TEST_CASE("Data.Zobrist.ZobristHash.Initialization", "[zobrist]") {
    CHECK(ZobristHash{}.hash() == 0);
    CHECK(ZobristHash::starting_position_hash().hash() != 0);

    const auto position = Position{FenString::starting_position()};
    const auto hash = ZobristHash::from_position(position);
    CHECK(hash == ZobristHash::starting_position_hash());
}

TEST_CASE("Data.Zobrist.ZobristHash.Swapping Side", "[zobrist]") {
    const auto hash = ZobristHash::starting_position_hash();
    auto hash2 = hash;
    hash2.swap_side();
    CHECK(hash2 != hash);
    hash2.swap_side();
    CHECK(hash2 == hash);
}

TEST_CASE("Data.Zobrist.ZobristHash.En Passant", "[zobrist]") {
    const auto hash = ZobristHash::starting_position_hash();
    auto hash2 = hash;
    hash2.set_enpassant(Rank{2});
    CHECK(hash2 != hash);
    hash2.clear_enpassant();
    CHECK(hash2 == hash);
    auto hash3 = hash;
    hash2.set_enpassant(Rank{6});
    hash3.set_enpassant(Rank{4});
    CHECK(hash2 != hash3);
    hash3.set_enpassant(Rank{6});
    CHECK(hash2 == hash3);
}

TEST_CASE("Data.Zobrist.ZobristHash.Castling", "[zobrist]") {
    CastlingRights rights1{true, false, false, false};
    CastlingRights rights2{true, true, true, false};
    CastlingRights rights3{false, false, true, true};

    const auto hash = ZobristHash::starting_position_hash();
    auto hash2 = hash;
    hash2.set_castling(rights1);
    CHECK(hash2 != hash);

    auto hash3 = hash;
    hash3.set_castling(rights2);
    CHECK(hash3 != hash2);

    auto hash4 = hash3;
    hash4.switch_castling(rights2, rights3);
    CHECK(hash4 != hash3);

    hash2.switch_castling(rights1, rights3);
    CHECK(hash2 != hash3);
    CHECK(hash2 == hash4);
}

TEST_CASE("Data.Zobrist.ZobristHash.Pieces", "[zobrist]") {
    const auto hash = ZobristHash::starting_position_hash();

    auto hash2 = hash;
    hash2.clear_piece(Piece::WhiteKnight, Square::B1);
    CHECK(hash2 != hash);

    auto hash3 = hash2;
    hash2.set_piece(Piece::WhiteKnight, Square::C3);
    CHECK(hash2 != hash);
    CHECK(hash2 != hash3);

    hash2.move_piece(Piece::WhiteKnight, Square::C3, Square::B1);
    CHECK(hash2 != hash3);
    CHECK(hash2 == hash);
}

TEST_CASE("Data.Zobrist.ZobristHash.Position", "[zobrist]") {
    const auto end_position = Position{FenString{"rn1qk2r/pb1ppp1p/2p2np1/8/4P3/1PP2N2/P4PPP/R1BQ1RK1 w kq - 1 10"}};
    const auto end_hash = ZobristHash::from_position(end_position);

    auto hash = ZobristHash::starting_position_hash();
    hash.move_piece(Piece::WhitePawn, Square::E2, Square::E4);
    hash.move_piece(Piece::BlackPawn, Square::G7, Square::G6);
    hash.move_piece(Piece::WhiteKnight, Square::B1, Square::C3);
    hash.move_piece(Piece::BlackBishop, Square::F8, Square::G7);
    hash.move_piece(Piece::WhiteKnight, Square::G1, Square::F3);
    hash.clear_piece(Piece::WhiteKnight, Square::C3);
    hash.move_piece(Piece::BlackBishop, Square::G7, Square::C3);
    hash.clear_piece(Piece::BlackBishop, Square::C3);
    hash.move_piece(Piece::WhitePawn, Square::D2, Square::C3);
    hash.move_piece(Piece::BlackKnight, Square::G8, Square::F6);
    hash.move_piece(Piece::WhiteBishop, Square::F1, Square::B5);
    hash.move_piece(Piece::BlackPawn, Square::C7, Square::C6);
    hash.move_piece(Piece::WhiteBishop, Square::B5, Square::A4);
    hash.move_piece(Piece::BlackPawn, Square::B7, Square::B5);
    hash.switch_castling(CastlingRights::all(), CastlingRights{false, false, true, true});
    hash.move_piece(Piece::WhiteKing, Square::E1, Square::G1);
    hash.move_piece(Piece::WhiteRook, Square::H1, Square::F1);
    hash.clear_piece(Piece::WhiteBishop, Square::A4);
    hash.move_piece(Piece::BlackPawn, Square::B5, Square::A4);
    hash.move_piece(Piece::WhitePawn, Square::B2, Square::B3);
    hash.clear_piece(Piece::WhitePawn, Square::B3);
    hash.move_piece(Piece::BlackPawn, Square::A4, Square::B3);
    hash.clear_piece(Piece::BlackPawn, Square::B3);
    hash.move_piece(Piece::WhitePawn, Square::C2, Square::B3);
    hash.move_piece(Piece::BlackBishop, Square::C8, Square::B7);

    CHECK(hash == end_hash);
}
