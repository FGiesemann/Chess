/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

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

auto piece_type_from_char(char piece_type) -> PieceType;

/**
 * \brief Color of a piece or player.
 */
enum class Color { White, Black };

/**
 * \brief A game piece.
 *
 * A piece is described by its type and color.
 */
struct Piece {
    PieceType type; ///< Type of the piece.
    Color color;    ///< Color of the piece.

    friend auto operator==(const Piece &lhs, const Piece &rhs) -> bool = default;
};

auto piece_from_fen_letter(char letter) -> Piece;

/**
 * \brief Describes the availability of castling for each player.
 */
struct CastlingAvailability {
    bool white_kingside{false};  ///< White can castle on the kingside.
    bool white_queenside{false}; ///< White can castle on the queenside.
    bool black_kingside{false};  ///< Black can castle on the kingside.
    bool black_queenside{false}; ///< Black can castle on the queenside.

    friend auto operator==(const CastlingAvailability &lhs, const CastlingAvailability &rhs) -> bool = default;

    auto operator[](char piece) const -> bool {
        switch (piece) {
        case 'K':
            return white_kingside;
        case 'Q':
            return white_queenside;
        case 'k':
            return black_kingside;
        case 'q':
            return black_queenside;
        default:
            return false;
        }
    }

    static auto all() -> CastlingAvailability { return CastlingAvailability{true, true, true, true}; }
    static auto none() -> CastlingAvailability { return CastlingAvailability{false, false, false, false}; }
};

/**
 * \brief A description of pieces on a chess board.
 *
 * This is a simple representation of the placement of pieces on a chess board.
 * The 64 squares are put into the list one rank after the other, starting with
 * rank 0.
 */
using PiecePlacement = std::array<std::optional<Piece>, 64>;

auto placement_from_string(const std::string &str) -> PiecePlacement;

auto starting_piece_placement() -> PiecePlacement;

} // namespace chesscore

#endif
