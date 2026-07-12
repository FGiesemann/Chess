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
class MagicBitboard {
public:
    /**
     * \brief An index into a magic bitboard.
     */
    struct Index {
        std::uint64_t value{}; ///< Value of the index.

        [[nodiscard]] auto index() const -> std::size_t { return value; }
    };

    /**
     * \brief Type for magic numbers.
     */
    struct MagicNumber {
        std::uint64_t value{}; ///< Value of the magic number.
    };

    /**
     * \brief Type for storing the attacker table.
     */
    using AttackerTable = Table<Bitmap, TableSize, Index>;

    /**
     * \brief Create a magic bitboard.
     *
     * \param magic_number Magic number for this magic bitboard.
     * \param shift Shift for this magic bitboard.
     * \param blocker_mask Blocker mask for this magic bitboard.
     */
    MagicBitboard(MagicNumber magic_number, std::size_t shift, const Bitmap &blocker_mask) : m_magic_number{magic_number}, m_shift{shift}, m_blocker_mask{blocker_mask} {}

    /**
     * \brief The mask to extract the blockers from the occupancy map.
     * \return Bitmask to extract blockers.
     */
    [[nodiscard]] auto blocker_mask() const -> Bitmap { return m_blocker_mask; }

    /**
     * \brief The attack bitmap for the sliding piece.
     *
     * The bitmap of all reachable squares, i.e. the attack bitmap, depends on
     * the distribution of blockers on the board.
     * \param blockers The blockers on the board.
     * \return The attack bitmap.
     */
    [[nodiscard]] auto reachable_squares(const Bitmap &blockers) const -> Bitmap { return attacker_bitmap(compute_index(blockers)); }

    /**
     * \brief The attack bitmap for the sliding piece.
     *
     * The bitmap of all reachable squares, i.e. the attack bitmap, depends on
     * the distribution of blockers on the board.
     * \param board The board describing the position of pieces.
     * \return The attack bitmap.
     */
    [[nodiscard]] auto reachable_squares(const Bitboard &board) const -> Bitmap {
        const auto blockers = board.occupancy() & m_blocker_mask;
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
    [[nodiscard]] auto compute_index(const Bitmap &blockers) const -> Index {
        Index index{};
        index.value = blockers.bits() * magic_number().value >> shift();
        return index;
    }

    /**
     * \brief Get the attacker bitmap from the table.
     *
     * \param index Index of the attacker bitmap.
     * \return The attacker bitmap.
     */
    [[nodiscard]] auto attacker_bitmap(const Index &index) const -> Bitmap { return m_attacker_table[index]; }

    /**
     * \brief The magic number used in this magic bitboard.
     * \return The magic number.
     */
    [[nodiscard]] auto magic_number() const -> MagicNumber { return m_magic_number; }

    /**
     * \brief The shift used in this magic bitboard.
     * \return The shift.
     */
    [[nodiscard]] auto shift() const -> std::uint64_t { return m_shift; }

    /**
     * \brief Access a table entry via index.
     * \param index The index.
     * \return The table entry.
     */
    [[nodiscard]] auto operator[](const Index &index) const -> const Bitmap & { return m_attacker_table[index]; }

    /**
     * \brief Access a table entry via index.
     * \param index The index.
     * \return The table entry.
     */
    auto operator[](const Index &index) -> Bitmap & { return m_attacker_table[index]; }

    /**
     * \brief The whole table of attack boards.
     *
     * Not every index of it might actually have an entry.
     * \return The table of attack bitmaps.
     */
    [[nodiscard]] auto table() const -> AttackerTable & { return m_attacker_table; }
private:
    MagicNumber m_magic_number; ///< The magic number used in hashing
    std::uint64_t m_shift;      ///< The shift used in hashing
    Bitmap m_blocker_mask;      ///< The mask for extracting blockers

    AttackerTable m_attacker_table{}; ///< The table of attack bitmaps
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

} // namespace chesscore

#endif
