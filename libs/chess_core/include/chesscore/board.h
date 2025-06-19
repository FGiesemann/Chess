/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#ifndef CHESSCORE_BOARD_H
#define CHESSCORE_BOARD_H

#include "chesscore/chesscore.h"

namespace chesscore {

/**
 * \brief Type of a piece.
 */
enum class PieceType { Pawn, Rook, Knight, Bishop, Queen, King };

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
};

class OutOfRange : public ChessException {
public:
    OutOfRange(const std::string &message) : ChessException{message} {}
};

/**
 * \brief A file (column) on the board.
 *
 * A file is a column on the board. It can be specified by a number 1..8 or by a
 * character a..h.
 */
struct File {
    /**
     * \brief A file from its name.
     *
     * The name is a character in the range a..h.
     * \param file A character in the range a..h
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
    char name() const;
};

/**
 * \brief A rank (row) on the board.
 *
 * A rank is a row on the board. It can be specified by a number 1..8.
 */
struct Rank {
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

/**
 * \brief Representation of a chessboard.
 */
class Board {};

} // namespace chesscore

#endif
