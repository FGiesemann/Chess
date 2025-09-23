/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_PIECE_H
#define CHESSCORE_PIECE_H

#include "chesscore/chesscore.h"
#include "chesscore/table.h"

#include <array>
#include <cstdint>
#include <optional>

namespace chesscore {

/**
 * \brief Type of a piece.
 */
enum class PieceType { Pawn, Rook, Knight, Bishop, Queen, King };

/**
 * \brief Number of available piece types.
 */
static constexpr int piece_type_count = 6;

/**
 * \brief All the piece types.
 *
 */
static constexpr std::array<PieceType, piece_type_count> all_piece_types{PieceType::Pawn, PieceType::Rook, PieceType::Knight, PieceType::Bishop, PieceType::Queen, PieceType::King};

/**
 * \brief Get the numeric index of a piece type.
 *
 * The piece types are enumerated pawn = 0, rook = 1, knight = 2, bishop = 3,
 * queen = 4, king = 5.
 * \param type The piece type.
 * \return The numerix index of the piece type.
 */
inline auto get_index(const PieceType &type) -> std::size_t {
    return static_cast<std::size_t>(type);
}

/**
 * \brief All the piece types that a pawn can promote into.
 */
static constexpr std::array<PieceType, 4> all_promotion_piece_types{PieceType::Rook, PieceType::Knight, PieceType::Bishop, PieceType::Queen};

/**
 * \brief Give a piece type from an index.
 *
 * The piece types are ordered as Pawn, Rook, Knight, Bishop, Queen, King.
 * \param index The index.
 * \return Piece type corresponding to the index.
 */
auto piece_type_from_index(std::size_t index) -> PieceType;

/**
 * \brief Converts a character to a piece type.
 *
 * Converts a character as it may appear in a FEN string (r, n, b, q, k, p) to
 * the corresponding piece type. The function handles lowercase and uppercase
 * letters.
 * \param letter The letter to be converted.
 * \return The piece type correspinding to the letter.
 * \throws ChessException If the letter does not signify a valid piece type.
 */
auto piece_type_from_char(char letter) -> PieceType;

/**
 * \brief Color of a piece or player.
 */
enum class Color { White, Black };

/**
 * \brief Swap a color.
 *
 * Switches white to black and black to white.
 * \param color The color to switch.
 * \return The switched color.
 */
inline auto other_color(Color color) -> Color {
    return color == Color::White ? Color::Black : Color::White;
}

/**
 * \brief A game piece.
 *
 * A piece is described by its type and color.
 */
struct Piece {
    PieceType type; ///< Type of the piece.
    Color color;    ///< Color of the piece.

    /**
     * \brief Equality comparison for pieces.
     *
     * Two pieces are equal if they have the same type and color.
     * @param lhs Left-hand side of the comparison.
     * @param rhs Right-hand side of the comparison.
     * @return Equality of the pieces.
     */
    friend auto operator==(const Piece &lhs, const Piece &rhs) -> bool = default;

    ///@{
    /**
     * \name Predefined chess pieces.
     * \brief A set of pre-defines chess pieces for each player.
     */
    static const Piece WhitePawn;   ///< A white pawn.
    static const Piece WhiteRook;   ///< A white rook.
    static const Piece WhiteKnight; ///< A white knight.
    static const Piece WhiteBishop; ///< A white bishop.
    static const Piece WhiteQueen;  ///< A white queen.
    static const Piece WhiteKing;   ///<  A white king.
    static const Piece BlackPawn;   ///< A black pawn.
    static const Piece BlackRook;   ///< A black rook.
    static const Piece BlackKnight; ///< A black knight.
    static const Piece BlackBishop; ///< A black bishop.
    static const Piece BlackQueen;  ///< A black queen.
    static const Piece BlackKing;   ///< A black king.
    ///@}

    auto piece_index() const -> size_t;
    auto piece_char() const -> char;
    auto piece_char_colorless() const -> char;
};

static constexpr size_t min_white_piece_index{0U};  ///< Index of the first white piece (white pawn)
static constexpr size_t max_white_piece_index{5U};  ///< Index of the last white peice (white king)
static constexpr size_t min_black_piece_index{6U};  ///< Index of the first black piece (black pawn)
static constexpr size_t max_black_piece_index{11U}; ///< Index of the last black piece (black king)

/**
 * \brief Converts a character to a piece.
 *
 * Converts a letter as it may appear in a FEN string (r, n, b, q, k, p) to a
 * chess piece. Lowercase letters stand for black pieces, uppercase letters
 * stand for white pieces.
 * \param letter The letter to be converted.
 * \return Piece Corresponding piece.
 * \throws ChessException If the letter does not signify a valid piece.
 */
auto piece_from_fen_letter(char letter) -> Piece;

/**
 * \brief A description of pieces on a chess board.
 *
 * This is a simple representation of the placement of pieces on a chess board.
 * The 64 squares are put into the list one rank after the other, starting with
 * rank 0.
 */
using PiecePlacement = std::array<std::optional<Piece>, 64>;

/**
 * \brief Generate a piece placement from a string.
 *
 * The string has to have length 64 and each character stand for a piece in one
 * of the squares of the chess board. It begins with position a1, then a2..a8,
 * b1..b8, ..., h1..h8.
 *
 * Each Character is either an underscore (_) denoting an empty square, or a
 * character p, r, n, b, q, k. Lowercase letters stand for black pieces,
 * uppercase letters stand for white pieces.
 *
 * \param str The string to interpret.
 * \return A corresponding piece placement.
 */
auto placement_from_string(const std::string &str) -> PiecePlacement;

/**
 * \brief Piece placement for the opening configuration of a regular chess game.
 *
 * Returns the piece placement for the opening configuration of a regular chess
 * game.
 * \return Piece placement for regular chess game.
 */
auto starting_piece_placement() -> PiecePlacement;

/**
 * \brief Type for a table of directions for each piece.
 *
 * The directions are stored in a bitfield.
 */
using PieceDirections = Table<std::uint8_t, piece_type_count, PieceType>;

/**
 * \brief A table storing ray directions for pieces.
 *
 * Stores the directions in which a sliding piee can move.
 */
extern const PieceDirections piece_ray_directions;

} // namespace chesscore

#endif
