/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_POSITION_TYPES_H
#define CHESSCORE_POSITION_TYPES_H

#include "chesscore/chesscore.h"
#include "chesscore/piece.h"
#include "chesscore/square.h"

namespace chesscore {

/**
 * \brief Describes the availability of castling for each player.
 */
class CastlingRights {
public:
    CastlingRights() = default;
    explicit CastlingRights(std::uint8_t rights) : m_rights{rights} {}

    static constexpr std::uint8_t white_king{0x01};
    static constexpr std::uint8_t white_queen{0x02};
    static constexpr std::uint8_t black_king{0x04};
    static constexpr std::uint8_t black_queen{0x08};

    [[nodiscard]] constexpr auto can_white_king() const -> bool { return (m_rights & white_king) > 0; }
    [[nodiscard]] constexpr auto can_white_queen() const -> bool { return (m_rights & white_queen) > 0; }
    [[nodiscard]] constexpr auto can_black_king() const -> bool { return (m_rights & black_king) > 0; }
    [[nodiscard]] constexpr auto can_black_queen() const -> bool { return (m_rights & black_queen) > 0; }

    constexpr auto set_white_king(bool allowed) -> void { m_rights = allowed ? (m_rights | white_king) : (m_rights & ~white_king); }
    constexpr auto set_white_queen(bool allowed) -> void { m_rights = allowed ? (m_rights | white_queen) : (m_rights & ~white_queen); }
    constexpr auto set_black_king(bool allowed) -> void { m_rights = allowed ? (m_rights | black_king) : (m_rights & ~black_king); }
    constexpr auto set_black_queen(bool allowed) -> void { m_rights = allowed ? (m_rights | black_queen) : (m_rights & ~black_queen); }

    static constexpr std::size_t max_combinations = 16ULL;

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
    auto operator[](char piece) const -> bool {
        switch (piece) {
        case 'K':
            return can_white_king();
        case 'Q':
            return can_white_queen();
        case 'k':
            return can_black_king();
        case 'q':
            return can_black_queen();
        default:
            throw OutOfRange("Invalid castling type");
        }
    }

    /**
     * \brief Generate an object with all castling rights.
     *
     * \return Object that has all the castling rights.
     */
    static auto all() -> CastlingRights { return CastlingRights{white_king | white_queen | black_king | black_queen}; }

    /**
     * \brief Generate an object with no castling rights.
     *
     * \return Object that has no castling rights.
     */
    static auto none() -> CastlingRights { return CastlingRights{}; }
private:
    std::uint8_t m_rights{};
};

static_assert(sizeof(CastlingRights) == 1, "CastlingRights must be 1 byte");

/**
 * \brief Possible check states of a position.
 */
enum class CheckState {
    None,      ///< No check.
    Check,     ///< Check.
    Checkmate, ///< Checkmate.
    Stalemate  ///< Stalemate.
};

/**
 * \brief Convert the check state into a symbol as used in SAN.
 *
 * Converts the CheckState into a single character symbol as it is used in SAN,
 * i.e.:
 * - '+' for CheckState::Check,
 * - '#' for CheckState::Checkmate,
 * - '' for CheckState::None and CheckState::Stalemate
 * \param state The state to convert.
 * \return The statae symbol.
 */
auto check_state_symbol(CheckState state) -> std::string;

struct PositionState {
    Color side_to_move{Color::White};                       ///< The player who moves next.
    int fullmove_number{1};                                 ///< Number of the next move.
    int halfmove_clock{0};                                  ///< Half-move clock for the fifty-move rule.
    CastlingRights castling_rights{CastlingRights::none()}; ///< Castling rights.
    Square en_passant_target{Square::None};                 ///< A possible en passant target square.

    /**
     * \brief Comparison of two PositionStates.
     *
     * \param rhs The state to compare to.
     * \return If the states are equal.
     */
    auto operator==(const PositionState &rhs) const -> bool;
};

} // namespace chesscore

#endif
