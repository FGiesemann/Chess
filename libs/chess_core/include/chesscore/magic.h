/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_MAGIC_H
#define CHESSCORE_MAGIC_H

#include "chesscore/bitboard.h"
#include "chesscore/bitmap.h"
#include "chesscore/position.h"
#include "chesscore/table.h"

namespace chesscore {

/**
 * \brief A table storing the attack bitboards for one piece type on a
 * particular suqare.
 *
 * The table is indexed by the blocker configuration around the sliding piece
 * (through hashing with a magic number).
 * \tparam TableSize Size of the table.
 */
template<std::size_t TableSize>
struct MagicBitboard {
    std::uint64_t magic_number{}; ///< The magic number used in hashing
    std::uint64_t shift{};        ///< The shift used in hashing
    Bitmap blocker_mask;          ///< The mask for extracting blockers

    /**
     * \brief Type for storing the attack table.
     */
    using AttackTable = Table<Bitmap, TableSize, std::uint64_t>;
    AttackTable attack_table{}; ///< The table of attack bitmaps

    /**
     * \brief The attack bitmap for the sliding piece.
     *
     * The bitmap of all reachable squares, i.e. the attack bitmap, depends on
     * the distribution of blockers on the board.
     * \param blockers The blockers on the board.
     * \return The attack bitmap.
     */
    [[nodiscard]] auto reachable_squares(const Bitmap &blockers) const -> Bitmap { return attack_table[compute_index(blockers)]; }

    /**
     * \brief The attack bitmap for the sliding piece.
     *
     * The bitmap of all reachable squares, i.e. the attack bitmap, depends on
     * the distribution of blockers on the board.
     * \param board The board describing the position of pieces.
     * \return The attack bitmap.
     */
    [[nodiscard]] auto reachable_squares(const Bitboard &board) const -> Bitmap {
        const auto blockers = board.occupancy() & blocker_mask;
        return reachable_squares(blockers);
    }

    /**
     * \brief The attack bitmap for the sliding piece.
     *
     * The bitmap of all reachable squares, i.e. the attack bitmap, depends on
     * the distribution of blockers on the board.
     * \param position The position describing the position of pieces on the board.
     * \return The attack bitmap.
     */
    [[nodiscard]] auto reachable_squares(const Position &position) const -> Bitmap { return reachable_squares(position.board()); }

    /**
     * \brief Compute the index from the blocker configuration.
     *
     * Applies the hash function to the blocker configuration to obtain the
     * index for the attacker bitmap.
     * \param blockers The blocker configuration.
     * \return The index of the attacker bitmap.
     */
    [[nodiscard]] auto compute_index(const Bitmap &blockers) const -> std::uint64_t { return blockers.bits() * magic_number >> shift; }
};

/**
 * \brief Compute the blocker mask for a sliding piece.
 *
 * Computes the mask of potential blockers for a sliding piece of the given
 * type, either Rook or Bishop, standing on the given square.
 * For other PieceTypes, an empty board is generated.
 * \param piece_type Type of the sliding piece.
 * \param square The starting square of the sliding piece.
 * \return The block mask for the piece.
 */
[[nodiscard]] auto blocker_mask(PieceType piece_type, const Square &square) -> Bitmap;

/**
 * \brief Compute the next blocker configuration.
 *
 * Repeatedly calling this function will generate all possible blocker
 * configurations for the given mask. The first call should be with
 * blockers = 0.
 * After a full cycle, the function generates blockers == 0 again.
 * \param blockers The current blocker configuration.
 * \param mask The blocker mask.
 * \return The next blocker configuration.
 */
[[nodiscard]] auto next_blocker_config(Bitmap blockers, Bitmap mask) -> Bitmap;

/**
 * \brief Generate the attack bitmap for a sliding piece.
 *
 * The attack bitmap describes all the squares that the sliding piece can reach
 * for the given configuration of blockers. If a square is occupied (blocked),
 * the square is considered accessible, i.e. no color checking is performed.
 * \param piece_type Type of the sliding piece.
 * \param square The starting square of the sliding piece.
 * \param blocker_config The configuration of blockers on the board.
 * \return The attack bitmap for the sliding piece and the blocker configuration.
 */
[[nodiscard]] auto attack_bitmap(PieceType piece_type, const Square &square, Bitmap blocker_config) -> Bitmap;

} // namespace chesscore

#endif
