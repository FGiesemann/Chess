/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#ifndef CHESSCORE_FEN_H
#define CHESSCORE_FEN_H

#include "chesscore/chesscore.h"
#include <string>

namespace chesscore {

class InvalidFen : public ChessException {
public:
    InvalidFen(const std::string &fen_string) : ChessException("Invalid FEN string: " + fen_string) {}
};

static constexpr std::string_view empty_fen = "8/8/8/8/8/8/8/8 w - - 0 1";
static constexpr std::string_view starting_position_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

class FenString {
public:
    FenString();
    explicit FenString(const std::string &fen_string);
    static auto startingPosition() -> FenString;
    auto str() const -> std::string { return fen_string; }
private:
    std::string fen_string;
};

namespace detail {

auto check_piece_placement(const std::string &fen_string) -> size_t;
auto check_side_to_move(const std::string &fen_string, size_t pos) -> size_t;
auto check_castling_availability(const std::string &fen_string, size_t pos) -> size_t;
auto check_en_passant_target_square(const std::string &fen_string, size_t pos) -> size_t;
auto check_halfmove_clock(const std::string &fen_string, size_t pos) -> size_t;
auto check_fullmove_number(const std::string &fen_string, size_t pos) -> void;

} // namespace detail

} // namespace chesscore

#endif
