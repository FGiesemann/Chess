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
 * \brief Compute the magic index.
 *
 * The index is derived from the blocker configuration through hashing with
 * a magic number.
 * \param blockers The blocker configuration.
 * \param magic_number The magic number.
 * \param shift The shift.
 * \return The index.
 */
inline auto magic_index(const Bitmap &blockers, std::uint64_t magic_number, std::uint8_t shift) -> std::uint64_t {
    return blockers.bits() * magic_number >> shift;
}

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
 * \brief The number of possible blocker configurations.
 *
 * \param blocker_mask The blocker mask.
 * \return The number of possible blocker configurations.
 */
[[nodiscard]] auto blocker_config_count(Bitmap blocker_mask) -> std::uint64_t;

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

/**
 * \brief Magic parameters for a sliding piece.
 *
 * The parameters describe the magic bitboard for a sliding piece on a single
 * square.
 */
struct Magics {
    Bitmap blocker_mask;          //< Mask to extract blockers from the occupancy map.
    std::uint64_t magic_number{}; //< The magic number.
    std::uint32_t offset{};       //< The offset of the attack map in the magic table.
    std::uint8_t shift{};         //< The shift for the index calculation.
};

/**
 * \brief Data for initializing magic tables.
 *
 * The data contains the magic number and shift to be used for initialization of
 * a magic table.
 */
struct MagicData {
    std::uint64_t magic_number{}; //< The magic number.
    std::uint32_t max_index{};    //< The highest index used.
    std::uint8_t shift{};         //< The shift used in index calculation.
};

/**
 * \brief A list of magic data for all squares.
 *
 * The list is indexed by square.
 */
using MagicDataSet = Table<MagicData, Square::count, Square>;

/**
 * \brief Compute the total size of the magic tables.
 *
 * \param data_set The data set.
 * \return The total number of entries.
 */
auto total_size(const MagicDataSet &data_set) -> std::size_t;

/**
 * \brief A magic bitboard.
 *
 * The magic bitboard contains the attack maps for a sliding piece.
 */
class MagicBitboard {
public:
    using MagicTable = Table<Magics, Square::count, Square>; //< Type of the list of magic parameters.

    MagicBitboard(PieceType piece_type, const MagicDataSet &data) : m_data_set{&data}, m_piece{piece_type} { init(); }

    /**
     * \brief Initialize the magic bitboard.
     *
     * Initializes the attack maps with the given data.
     */
    auto init() -> void;

    /**
     * \brief Access the table of magic parameters.
     *
     * \return The table of magic parameters.
     */
    [[nodiscard]] auto magics() const -> const MagicTable & { return m_magics; }

    /**
     * \brief Access the table of magic parameters.
     *
     * \return The table of magic parameters.
     */
    [[nodiscard]] auto magics() -> MagicTable & { return m_magics; }

    /**
     * \brief Get the attack map.
     *
     * Returns the attack map for the given square and position.
     * \param square The square of the sliding piece.
     * \param position The position.
     * \return The attack map.
     */
    [[nodiscard]] auto attacks(const Square &square, const Position &position) const -> const Bitmap & { return attacks(square, position.board()); }

    /**
     * \brief Get the attack map.
     *
     * Returns the attack map for the given square and bitboard.
     * \param square The square of the sliding piece.
     * \param bitboard The bitboard.
     * \return The attack map.
     */
    [[nodiscard]] auto attacks(const Square &square, const Bitboard &bitboard) const -> const Bitmap & {
        const auto &magic = magics()[square];
        return m_attack_maps[magic.offset + magic_index(bitboard.occupancy() & magic.blocker_mask, magic.magic_number, magic.shift)];
    }
private:
    const MagicDataSet *m_data_set;    //< Reference to the data set for initialization. (non-owning)
    PieceType m_piece;                 //< The piece type.
    std::vector<Bitmap> m_attack_maps; //< The list of attack maps.
    MagicTable m_magics;               //< The list of magic parameters.

    /**
     * \brief Fill the attack map.
     *
     * Computes the attack maps from the given initialization data and store
     * them in the list of attack maps.
     * \param magics The magc parameters.
     * \param square The square.
     * \param offset The offset in the list of attack maps.
     */
    auto fill_table(const Magics &magics, const Square &square, std::uint32_t offset) -> void;
};

} // namespace chesscore

#include "magic_data.h"

namespace chesscore {

inline const MagicBitboard magic_rook_bitboard{PieceType::Rook, magic_rook_data};
inline const MagicBitboard magic_bishop_bitboard{PieceType::Bishop, magic_bishop_data};

} // namespace chesscore

#endif
