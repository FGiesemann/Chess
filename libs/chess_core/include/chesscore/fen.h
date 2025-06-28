/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_FEN_H
#define CHESSCORE_FEN_H

#include "chesscore/chesscore.h"
#include "chesscore/piece.h"
#include "chesscore/square.h"

#include <optional>
#include <string>

namespace chesscore {

/**
 * \brief Exception thrown when an invalid FEN string is encountered.
 */
class InvalidFen : public ChessException {
public:
    /**
     * \brief Create an exception with a message.
     *
     * @param message The message.
     */
    InvalidFen(const std::string &message) : ChessException("Invalid FEN string: " + message) {}
};

static constexpr std::string_view empty_fen = "8/8/8/8/8/8/8/8 w - - 0 1"; ///< FEN string for an empy board.
static constexpr std::string_view starting_position_fen =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; ///< FEN string for the starting configuration of a chess game.

/**
 * \brief Interpreter for FEN strings.
 *
 * The Forsythe–Edwards Notation (FEN) is a standard notation for describing a
 * configuration of a chess game. It describes the piece placement, the sife to
 * play next, castling availabilities for the players, en passant possibility,
 * the halfmove clock, and the fullmove number.
 *
 * This class checks the validity of a FEN string and makes the information from
 * it available to the user.
 */
class FenString {
public:
    /**
     * \brief A FEN string representing an empty board.
     *
     * Generates a FEN string that describes an empty board.
     */
    FenString();

    /**
     * \brief Checks and extract information from a FEN string.
     *
     * This cheks the FEN string for validity and extracts information from it,
     * that can then be queried via member functions.
     * \param fen_string The FEN string to check.
     */
    explicit FenString(const std::string &fen_string);

    /**
     * \brief Generate a FEN strig for the starting position.
     *
     * This function constructs a FEN string object that escribes the starting
     * configuration of a regaular chess game.
     * \return FEN string for the starting position.
     */
    static auto starting_position() -> FenString;

    /**
     * \brief Access the underlying string.
     *
     * This gives access to the string representation of the FEN string.
     * \return The string representation of the FEN string.
     */
    auto str() const -> const std::string & { return m_fen_string; }

    /**
     * \brief The piece placement.
     *
     * Access the piece placement as extracted from the FEN string.
     * \return Piece placement.
     */
    auto piece_placement() const -> const PiecePlacement & { return m_piece_placement; }

    /**
     * \brief The side to move next.
     *
     * Access the side to move next as extracted from the FEN string.
     * \return The side to move next.
     */
    auto side_to_move() const -> Color { return m_side_to_move; }

    /**
     * \brief The castling rights.
     *
     * Access the castling rights for both players as extracted from the FEN
     * string.
     * \return The castling rights.
     */
    auto castling_rights() const -> CastlingRights { return m_castling_rights; }

    /**
     * \brief The en passant target square.
     *
     * If the FEN string describes a position where en passant is possible, it
     * is returned here. Otherwise a nullopt is returned.
     * \return Potential en passant target square.
     */
    auto en_passant_square() const -> std::optional<Square> { return m_en_passant; }

    /**
     * \brief Value of the halfmove clock.
     *
     * Access the value of the halfmove clock as extracted from the FEN string.
     * \return The value of the halfmove clock.
     */
    auto halfmove_clock() const -> size_t { return m_halfmove_clock; }

    /**
     * \brief The fullmove number.
     *
     * Access the fullmove number as extracted from the FEN string.
     * \return The fullmove number.
     */
    auto fullmove_number() const -> size_t { return m_fullmove_number; }
private:
    std::string m_fen_string;

    PiecePlacement m_piece_placement;
    Color m_side_to_move;
    CastlingRights m_castling_rights;
    std::optional<Square> m_en_passant;
    int m_halfmove_clock;
    int m_fullmove_number;
};

namespace detail {

auto check_piece_placement(const std::string &fen_string) -> std::pair<PiecePlacement, std::size_t>;
auto check_side_to_move(const std::string &fen_string, std::size_t pos) -> std::pair<Color, std::size_t>;
auto check_castling_availability(const std::string &fen_string, std::size_t pos) -> std::pair<CastlingRights, std::size_t>;
auto check_en_passant_target_square(const std::string &fen_string, Color player_to_move, std::size_t pos) -> std::pair<std::optional<Square>, std::size_t>;
auto check_halfmove_clock(const std::string &fen_string, std::size_t pos) -> std::pair<int, std::size_t>;
auto check_fullmove_number(const std::string &fen_string, std::size_t pos) -> int;

} // namespace detail

} // namespace chesscore

#endif
