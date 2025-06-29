/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <catch2/catch_all.hpp>

#include "chesscore/fen.h"

using namespace chesscore;

TEST_CASE("Data.FEN.Empty FEN string", "[FENString][Init]") {
    FenString empty_board{};
    CHECK(empty_board.str() == "8/8/8/8/8/8/8/8 w - - 0 1");
    CHECK(empty_board.side_to_move() == Color::White);
    CHECK(empty_board.en_passant_square() == std::nullopt);
    CHECK(empty_board.halfmove_clock() == 0);
    CHECK(empty_board.fullmove_number() == 1);
}

TEST_CASE("Data.FEN.Starting position FEN string", "[FENString][Init]") {
    FenString starting_position{FenString::starting_position()};
    CHECK(starting_position.str() == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    CHECK(starting_position.side_to_move() == Color::White);
    CHECK(starting_position.en_passant_square() == std::nullopt);
    CHECK(starting_position.halfmove_clock() == 0);
    CHECK(starting_position.fullmove_number() == 1);
}

TEST_CASE("Data.FEN.Piece placement", "[FENString][Validity]") {
    static const PiecePlacement pieces_a = placement_from_string("RNBQKBNRPPPPPPPP________________________p_______ppppppp_rnbqkbnr");
    static const PiecePlacement pieces_b = placement_from_string("RNBQKBNRPPPPPPPP______________________________p_ppppppp_rnbqkbnr");

    CHECK(detail::check_piece_placement("8/8/8/8/8/8/8/8 w - - 0 1") == std::make_pair(PiecePlacement{}, size_t{16}));
    CHECK(detail::check_piece_placement("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") == std::make_pair(starting_piece_placement(), size_t{44}));
    CHECK(detail::check_piece_placement("rnbqkbnr/ppppppp1/p7/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") == std::make_pair(pieces_a, size_t{45}));
    CHECK(detail::check_piece_placement("rnbqkbnr/ppppppp1/6p1/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") == std::make_pair(pieces_b, size_t{46}));

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

TEST_CASE("Data.FEN.Side to move", "[FENString][Validity]") {
    CHECK(detail::check_side_to_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 44) == std::make_pair(Color::White, size_t{46}));
    CHECK(detail::check_side_to_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1", 44) == std::make_pair(Color::Black, size_t{46}));
    CHECK(detail::check_side_to_move("8/8/8/8/8/8/8/8 w - - 0 1", 16) == std::make_pair(Color::White, size_t{18}));

    // Invalid side to move
    CHECK_THROWS_AS(detail::check_side_to_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR x KQkq - 0 1", 44), InvalidFen);  // invalid color
    CHECK_THROWS_AS(detail::check_side_to_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR ww KQkq - 0 1", 44), InvalidFen); // two letters

    // Invalid start pos
    CHECK_THROWS_AS(detail::check_side_to_move("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b KQkq - 0 1", 34), InvalidFen);
}

TEST_CASE("Data.FEN.Castling availability", "[FENString][Validity]") {
    CHECK(detail::check_castling_availability("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 46) == std::make_pair(CastlingRights::all(), size_t{51}));
    CHECK(detail::check_castling_availability("8/8/8/8/8/8/8/8 w - - 0 1", 18) == std::make_pair(CastlingRights::none(), size_t{20}));

    // Invalid castling specification
    CHECK_THROWS_AS(detail::check_castling_availability("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KPkq - 0 1", size_t{45}), InvalidFen);  // invalid piece
    CHECK_THROWS_AS(detail::check_castling_availability("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkqk - 0 1", size_t{45}), InvalidFen); // too many pieces
    CHECK_THROWS_AS(detail::check_castling_availability("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KkQq - 0 1", size_t{45}), InvalidFen);  // invalid order
}

TEST_CASE("Data.FEN.En passant square", "[FENString][Validity]") {
    CHECK(
        detail::check_en_passant_target_square("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", Color::White, 51) == std::make_pair(std::optional<Square>{}, size_t{53})
    );
    CHECK(detail::check_en_passant_target_square("8/8/8/8/8/8/8/8 w - - 0 1", Color::White, 20) == std::make_pair(std::optional<Square>{}, size_t{22}));
    CHECK(
        detail::check_en_passant_target_square("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq c6 0 1", Color::White, 51) ==
        std::make_pair(std::optional{Square::C6}, size_t{54})
    );
    CHECK(detail::check_en_passant_target_square("8/8/8/8/8/8/8/8 b - f3 0 1", Color::Black, 20) == std::make_pair(std::optional{Square::F3}, size_t{23}));
    CHECK(detail::check_en_passant_target_square("8/8/8/8/8/8/8/8 w - f6 0 1", Color::White, 20) == std::make_pair(std::optional{Square::F6}, size_t{23}));

    CHECK_THROWS_AS(detail::check_en_passant_target_square("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq t 0 1", Color::White, 51), InvalidFen);   // invalid character
    CHECK_THROWS_AS(detail::check_en_passant_target_square("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq e 0 1", Color::White, 51), InvalidFen);   // incomplete
    CHECK_THROWS_AS(detail::check_en_passant_target_square("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq n3 0 1", Color::White, 51), InvalidFen);  // invalid file
    CHECK_THROWS_AS(detail::check_en_passant_target_square("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq c4 0 1", Color::White, 51), InvalidFen);  // invalid rank
    CHECK_THROWS_AS(detail::check_en_passant_target_square("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq c3w 0 1", Color::White, 51), InvalidFen); // too long
}

TEST_CASE("Data.FEN.Halfmove clock", "[FENString][Validity]") {
    CHECK(detail::check_halfmove_clock("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 53) == std::make_pair(0, size_t{55}));
    CHECK(detail::check_halfmove_clock("8/8/8/8/8/8/8/8 w - - 0 1", 22) == std::make_pair(0, size_t{24}));
    CHECK(detail::check_halfmove_clock("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 26 1", 53) == std::make_pair(26, size_t{56}));
    CHECK(detail::check_halfmove_clock("8/8/8/8/8/8/8/8 w - - 2 1", 22) == std::make_pair(2, size_t{24}));
    CHECK(detail::check_halfmove_clock("8/8/8/8/8/8/8/8 w - - 236 1", 22) == std::make_pair(236, size_t{26}));

    CHECK_THROWS_AS(detail::check_halfmove_clock("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - x 1", 53), InvalidFen);  // non-digit
    CHECK_THROWS_AS(detail::check_halfmove_clock("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 3_ 1", 53), InvalidFen); // non-digit
    CHECK_THROWS_AS(detail::check_halfmove_clock("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 3_ 1", 53), InvalidFen); // non-digit
    CHECK_THROWS_AS(detail::check_halfmove_clock("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 31", 53), InvalidFen);   // unexpected end of FEN
}

TEST_CASE("Data.FEN.Full move number", "[FENString][Validity]") {
    CHECK_NOTHROW(detail::check_fullmove_number("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", size_t{55}));
    CHECK_NOTHROW(detail::check_fullmove_number("8/8/8/8/8/8/8/8 w - - 0 1", size_t{24}));

    CHECK_THROWS_AS(detail::check_fullmove_number("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 g", 55), InvalidFen);    // non-digit
    CHECK_THROWS_AS(detail::check_fullmove_number("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 ", 55), InvalidFen);     // missing number
    CHECK_THROWS_AS(detail::check_fullmove_number("8/8/8/8/8/8/8/8 w - - 0 1 ", 24), InvalidFen);                                  // FEN string too long
    CHECK_THROWS_AS(detail::check_fullmove_number("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 46", 55), InvalidFen); // FEN string too long
}

TEST_CASE("Data.FEN.Valid FEN strings", "[FENString][Validity]") {
    // 1. e4
    FenString fen;
    CHECK_NOTHROW(fen = FenString("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"));
    CHECK(fen.side_to_move() == Color::Black);
    CHECK(fen.castling_rights() == CastlingRights::all());
    CHECK(fen.en_passant_square() == Square::E3);
    CHECK(fen.halfmove_clock() == 0);
    CHECK(fen.fullmove_number() == 1);
    // 1. ... c5
    CHECK_NOTHROW(fen = FenString{"rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"});
    CHECK(fen.side_to_move() == Color::White);
    CHECK(fen.castling_rights() == CastlingRights::all());
    CHECK(fen.en_passant_square() == Square::C6);
    CHECK(fen.halfmove_clock() == 0);
    CHECK(fen.fullmove_number() == 2);

    // 2. Nf3
    CHECK_NOTHROW(fen = FenString{"rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"});
    CHECK(fen.side_to_move() == Color::Black);
    CHECK(fen.castling_rights() == CastlingRights::all());
    CHECK_FALSE(fen.en_passant_square().has_value());
    CHECK(fen.halfmove_clock() == 1);
    CHECK(fen.fullmove_number() == 2);

    CHECK_NOTHROW(fen = FenString{"4k3/8/8/8/8/8/4P3/4K3 w - - 5 39"});
    CHECK(fen.side_to_move() == Color::White);
    CHECK(fen.castling_rights() == CastlingRights::none());
    CHECK_FALSE(fen.en_passant_square().has_value());
    CHECK(fen.halfmove_clock() == 5);
    CHECK(fen.fullmove_number() == 39);
}
