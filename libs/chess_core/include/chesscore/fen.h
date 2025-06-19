/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#ifndef CHESSCORE_FEN_H
#define CHESSCORE_FEN_H

#include "chesscore/chesscore.h"
#include "chesscore/piece.h"
#include "chesscore/square.h"

#include <optional>
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
    static auto starting_position() -> FenString;
    auto str() const -> std::string { return m_fen_string; }

    auto side_to_move() const -> Color { return m_side_to_move; }
    auto en_passant_square() const -> std::optional<Square> { return m_en_passant; }
    auto halfmove_clock() const -> size_t { return m_halfmove_clock; }
    auto fullmove_number() const -> size_t { return m_fullmove_number; }
private:
    std::string m_fen_string;

    Color m_side_to_move;
    std::optional<Square> m_en_passant;
    int m_halfmove_clock;
    int m_fullmove_number;
};

namespace detail {

auto check_piece_placement(const std::string &fen_string) -> std::size_t;
auto check_side_to_move(const std::string &fen_string, std::size_t pos) -> std::pair<Color, std::size_t>;
// TODO: extract castling availability
auto check_castling_availability(const std::string &fen_string, std::size_t pos) -> std::size_t;
// TODO: check dependent on side_to_move
auto check_en_passant_target_square(const std::string &fen_string, std::size_t pos) -> std::pair<std::optional<Square>, std::size_t>;
auto check_halfmove_clock(const std::string &fen_string, std::size_t pos) -> std::pair<int, std::size_t>;
auto check_fullmove_number(const std::string &fen_string, std::size_t pos) -> int;

} // namespace detail

} // namespace chesscore

#endif
