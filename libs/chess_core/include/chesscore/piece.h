/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_PIECE_H
#define CHESSCORE_PIECE_H

#include "chesscore/chesscore.h"

#include <array>
#include <optional>

namespace chesscore {

/**
 * \brief Type of a piece.
 */
enum class PieceType { Pawn, Rook, Knight, Bishop, Queen, King };

static constexpr int piece_type_count = 6;

/**
 * \brief Give a piece type from an index.
 *
 * The piece types are ordered as Pawn, Rook, Knight, Bishop, Queen, King.
 * \param index The index.
 * \return Piece type corresponding to the index.
 */
auto piece_type_from_index(int index) -> PieceType;

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
};

static constexpr int min_white_piece_index = 0;
static constexpr int max_white_piece_index = 5;
static constexpr int min_black_piece_index = 6;
static constexpr int max_black_piece_index = 11;

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

} // namespace chesscore

#endif
