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

    auto piece_placement() const -> const PiecePlacement & { return m_piece_placement; }
    auto side_to_move() const -> Color { return m_side_to_move; }
    auto castling_availability() const -> CastlingAvailability { return m_castling_availability; }
    auto en_passant_square() const -> std::optional<Square> { return m_en_passant; }
    auto halfmove_clock() const -> size_t { return m_halfmove_clock; }
    auto fullmove_number() const -> size_t { return m_fullmove_number; }
private:
    std::string m_fen_string;

    PiecePlacement m_piece_placement;
    Color m_side_to_move;
    CastlingAvailability m_castling_availability;
    std::optional<Square> m_en_passant;
    int m_halfmove_clock;
    int m_fullmove_number;
};

namespace detail {

auto check_piece_placement(const std::string &fen_string) -> std::pair<PiecePlacement, std::size_t>;
auto check_side_to_move(const std::string &fen_string, std::size_t pos) -> std::pair<Color, std::size_t>;
auto check_castling_availability(const std::string &fen_string, std::size_t pos) -> std::pair<CastlingAvailability, std::size_t>;
auto check_en_passant_target_square(const std::string &fen_string, Color player_to_move, std::size_t pos) -> std::pair<std::optional<Square>, std::size_t>;
auto check_halfmove_clock(const std::string &fen_string, std::size_t pos) -> std::pair<int, std::size_t>;
auto check_fullmove_number(const std::string &fen_string, std::size_t pos) -> int;

} // namespace detail

} // namespace chesscore

#endif
