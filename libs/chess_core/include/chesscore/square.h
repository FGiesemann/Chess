/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#ifndef CHESSCORE_SQUARE_H
#define CHESSCORE_SQUARE_H

#include "chesscore/chesscore.h"

namespace chesscore {

namespace detail {

constexpr auto charToLower(const unsigned char character) -> unsigned char {
    return (character >= 'A' && character <= 'Z') ? character + ('a' - 'A') : character;
}

} // namespace detail

/**
 * \brief A file (column) on the board.
 *
 * A file is a column on the board. It can be specified by a number 1..8 or by a
 * character a..h.
 */
struct File {
    static constexpr int min_file = 1; ///< The minimum allowed file number.
    static constexpr int max_file = 8; ///< The maximum allowed file number.

    /**
     * \brief A file from its name.
     *
     * The name is a character in the range a..h. The name is case insensitive,
     * so 'A'..'H' are also valid.
     * \param file A character in the range a..h (case insensitive).
     */
    constexpr File(char file) : file{static_cast<int>(detail::charToLower(file) - 'a') + 1} {
        if (this->file < File::min_file || this->file > File::max_file) {
            throw OutOfRange{"File must be between 'a' and 'h'"};
        }
    }

    /**
     * \brief A file from its number.
     *
     * The file is specified as a number in the range 1..8.
     * \param file A number in the range 1..8.
     */
    constexpr File(int file) : file{file} {
        if (file < File::min_file || file > File::max_file) {
            throw OutOfRange{"File must be between 1 and 8"};
        }
    }

    int file; ///< The file number (1..8).

    /**
     * \brief Returns the file as a character.
     *
     * The name of the file is a character in the range a..h.
     * \return The name of the file.
     */
    [[nodiscard]] auto name() const -> char;

    /**
     * \brief Equality comparison for files.
     *
     * Compare two file numbers for equality. They are equal, if they represent
     * the same file.
     * @param lhs Left-hand side of the comparison.
     * @param rhs Right-hand side of the comparison.
     * @return Equality of the two files.
     */
    friend auto operator==(const File &lhs, const File &rhs) -> bool { return lhs.file == rhs.file; }
};

/**
 * \brief A rank (row) on the board.
 *
 * A rank is a row on the board. It can be specified by a number 1..8.
 */
struct Rank {
    static constexpr int min_rank = 1; ///< The minimum allowed rank number.
    static constexpr int max_rank = 8; ///< The maximum allowed rank number.

    /**
     * \brief A rank from its number.
     *
     * The rank is a number in the range 1..8.
     * \param rank A number in the range 1..8.
     */
    constexpr Rank(int rank) : rank{rank} {
        if (rank < Rank::min_rank || rank > Rank::max_rank) {
            throw OutOfRange{"Rank must be between 1 and 8"};
        }
    }

    int rank; ///< The rank number (1..8).

    /**
     * \brief Equality comparison for ranks.
     *
     * Compare two rank numbers for equality. They are equal, if they represent
     * the same rank.
     * @param lhs Left-hand side of the comparison.
     * @param rhs Right-hand side of the comparison.
     * @return Equality of the two ranks.
     */
    friend auto operator==(const Rank &lhs, const Rank &rhs) -> bool { return lhs.rank == rhs.rank; }
};

/**
 * \brief A position on the board.
 *
 * Each square is identified by its file (the column) and the rank (the row).
 */
struct Square {
    File file; ///< The file (column) of the square.
    Rank rank; ///< The rank (row) of the square.

    /**
     * \brief Equality comparison for square positions.
     *
     * Compare two square positions for equality. They are equal, if their files
     * and ranks are equal.
     * @param lhs Left-hand side of the comparison.
     * @param rhs Right-hand side of the comparison.
     * @return Equality of the two square positions.
     */
    friend auto operator==(const Square &lhs, const Square &rhs) -> bool { return lhs.file == rhs.file && lhs.rank == rhs.rank; }

    static const Square A1, A2, A3, A4, A5, A6, A7, A8;
    static const Square B1, B2, B3, B4, B5, B6, B7, B8;
    static const Square C1, C2, C3, C4, C5, C6, C7, C8;
    static const Square D1, D2, D3, D4, D5, D6, D7, D8;
    static const Square E1, E2, E3, E4, E5, E6, E7, E8;
    static const Square F1, F2, F3, F4, F5, F6, F7, F8;
    static const Square G1, G2, G3, G4, G5, G6, G7, G8;
    static const Square H1, H2, H3, H4, H5, H6, H7, H8;
};

} // namespace chesscore

#endif
