/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#ifndef CHESSCORE_SQUARE_H
#define CHESSCORE_SQUARE_H

#include "chesscore/chesscore.h"

namespace chesscore {

/**
 * \brief A file (column) on the board.
 *
 * A file is a column on the board. It can be specified by a number 1..8 or by a
 * character a..h.
 */
struct File {
    static constexpr int min_file = 1;
    static constexpr int max_file = 8;

    /**
     * \brief A file from its name.
     *
     * The name is a character in the range a..h. The name is case insensitive,
     * so 'A'..'H' are also valid.
     * \param file A character in the range a..h (case insensitive).
     */
    explicit File(char file);

    /**
     * \brief A file from its number.
     *
     * The file is specified as a number in the range 1..8.
     * \param file A number in the range 1..8.
     */
    explicit File(int file);

    int file; ///< The file number (1..8).

    /**
     * \brief Returns the file as a character.
     *
     * The name of the file is a character in the range a..h.
     * \return The name of the file.
     */
    [[nodiscard]] auto name() const -> char;
};

/**
 * \brief A rank (row) on the board.
 *
 * A rank is a row on the board. It can be specified by a number 1..8.
 */
struct Rank {
    static constexpr int min_rank = 1;
    static constexpr int max_rank = 8;

    /**
     * \brief A rank from its number.
     *
     * The rank is a number in the range 1..8.
     * \param rank A number in the range 1..8.
     */
    explicit Rank(int rank);
    int rank; ///< The rank number (1..8).
};

/**
 * \brief A position on the board.
 *
 * Each square is identified by its file (the column) and the rank (the row).
 */
struct Square {
    File file;
    Rank rank;
};

} // namespace chesscore

#endif
