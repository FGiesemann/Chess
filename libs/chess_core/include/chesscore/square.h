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

    ///@{
    /**
     * \name Squares on a chess board
     * \brief Pre-defined square positions on a chess board.
     *
     * The square names correspond with the default naming of chess boards.
     */
    static const Square A1; ///< The square A1.
    static const Square A2; ///< The square A2.
    static const Square A3; ///< The square A3.
    static const Square A4; ///< The square A4.
    static const Square A5; ///< The square A5.
    static const Square A6; ///< The square A6.
    static const Square A7; ///< The square A7.
    static const Square A8; ///< The square A8.
    static const Square B1; ///< The square B1.
    static const Square B2; ///< The square B2.
    static const Square B3; ///< The square B3.
    static const Square B4; ///< The square B4.
    static const Square B5; ///< The square B5.
    static const Square B6; ///< The square B6.
    static const Square B7; ///< The square B7.
    static const Square B8; ///< The square B8.
    static const Square C1; ///< The square C1.
    static const Square C2; ///< The square C2.
    static const Square C3; ///< The square C3.
    static const Square C4; ///< The square C4.
    static const Square C5; ///< The square C5.
    static const Square C6; ///< The square C6.
    static const Square C7; ///< The square C7.
    static const Square C8; ///< The square C8.
    static const Square D1; ///< The square D1.
    static const Square D2; ///< The square D2.
    static const Square D3; ///< The square D3.
    static const Square D4; ///< The square D4.
    static const Square D5; ///< The square D5.
    static const Square D6; ///< The square D6.
    static const Square D7; ///< The square D7.
    static const Square D8; ///< The square D8.
    static const Square E1; ///< The square E1.
    static const Square E2; ///< The square E2.
    static const Square E3; ///< The square E3.
    static const Square E4; ///< The square E4.
    static const Square E5; ///< The square E5.
    static const Square E6; ///< The square E6.
    static const Square E7; ///< The square E7.
    static const Square E8; ///< The square E8.
    static const Square F1; ///< The square F1.
    static const Square F2; ///< The square F2.
    static const Square F3; ///< The square F3.
    static const Square F4; ///< The square F4.
    static const Square F5; ///< The square F5.
    static const Square F6; ///< The square F6.
    static const Square F7; ///< The square F7.
    static const Square F8; ///< The square F8.
    static const Square G1; ///< The square G1.
    static const Square G2; ///< The square G2.
    static const Square G3; ///< The square G3.
    static const Square G4; ///< The square G4.
    static const Square G5; ///< The square G5.
    static const Square G6; ///< The square G6.
    static const Square G7; ///< The square G7.
    static const Square G8; ///< The square G8.
    static const Square H1; ///< The square H1.
    static const Square H2; ///< The square H2.
    static const Square H3; ///< The square H3.
    static const Square H4; ///< The square H4.
    static const Square H5; ///< The square H5.
    static const Square H6; ///< The square H6.
    static const Square H7; ///< The square H7.
    static const Square H8; ///< The square H8.
    ///@}
};

} // namespace chesscore

#endif
