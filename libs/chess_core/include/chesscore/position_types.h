/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_POSITION_TYPES_H
#define CHESSCORE_POSITION_TYPES_H

#include "chesscore/piece.h"
#include "chesscore/square.h"

namespace chesscore {

/**
 * \brief Describes the availability of castling for each player.
 */
struct CastlingRights {
    bool white_kingside{false};  ///< White can castle on the kingside.
    bool white_queenside{false}; ///< White can castle on the queenside.
    bool black_kingside{false};  ///< Black can castle on the kingside.
    bool black_queenside{false}; ///< Black can castle on the queenside.

    /**
     * \brief Quality comparison for castling availability.
     *
     * Two castling availability objects are equal if they have the same
     * castling rights for each player.
     * \param lhs Left-hand side of the comparison.
     * \param rhs Right-hand side of the comparison.
     * \return Equality of the two objects.
     */
    friend auto operator==(const CastlingRights &lhs, const CastlingRights &rhs) -> bool = default;

    /**
     * \brief Get the castling rights for a a player.
     *
     * Query the castling rights for a player and a side (kingside/queenside).
     * The letter given has the same meaning as in a FEN string. (K = kingside
     * castling of the white king, q = queenside castling of the black king,
     * ...)
     * \param piece The castling type as described above.
     * @return If the castling right is available.
     */
    auto operator[](char piece) const -> const bool & {
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
            throw OutOfRange("Invalid castling type");
        }
    }

    /**
     * \brief Get the castling rights for a a player.
     *
     * Query the castling rights for a player and a side (kingside/queenside).
     * The letter given has the same meaning as in a FEN string. (K = kingside
     * castling of the white king, q = queenside castling of the black king,
     * ...)
     * \param piece The castling type as described above.
     * @return If the castling right is available.
     */
    auto operator[](char piece) -> bool & {
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
            throw OutOfRange("Invalid castling type");
        }
    }

    /**
     * \brief Generate an object with all castling rights.
     *
     * \return Object that has all the castling rights.
     */
    static auto all() -> CastlingRights { return CastlingRights{true, true, true, true}; }

    /**
     * \brief Generate an object with no castling rights.
     *
     * \return Object that has no castling rights.
     */
    static auto none() -> CastlingRights { return CastlingRights{false, false, false, false}; }
};

struct PositionState {
    Color side_to_move{Color::White};                       ///< The player who moves next.
    size_t fullmove_number{1};                              ///< Number of the next move.
    size_t halfmove_clock{0};                               ///< Half-move clock for the fifty-move rule.
    CastlingRights castling_rights{CastlingRights::none()}; ///< Castling rights.
    std::optional<Square> en_passant_target{};              ///< A possible en passant target square.
};

} // namespace chesscore

#endif
