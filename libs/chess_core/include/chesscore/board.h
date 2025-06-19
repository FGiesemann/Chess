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
    PieceType type; //< Type of the piece.
    Color color;    //< Color of the piece.
};

class InvalidSquare : public ChessException {
public:
    InvalidSquare(bool rank_invalid, bool file_invalid);

    bool is_rank_invalid() const { return m_rank_invalid; }
    bool is_file_invalid() const { return m_file_invalid; }
private:
    bool m_rank_invalid;
    bool m_file_invalid;
};

/**
 * \brief A position on the board.
 *
 * Each square is identified by its file (the column) and the rank (the row).
 */
class Square {
public:
    /**
     * \brief Define a square by its file and rank.
     *
     * File and rank are numbers from 1 to 8 (inclusive).
     *
     * \param file File (column) of the square in the range 1..8.
     * \param rank Rank (row) of the square in the range 1..8.
     * \throws InvalidSquare if the file or rank is out of range.
     * \return A square with the given file and rank.
     */
    static Square from_coords(int file, int rank);

    /**
     * \brief Define a square by its file and rank.
     *
     * The file is given as a letter from a to h, the rank is given as a number
     * from 1 to 8.
     *
     * \param file File (column) of the square as a letter 'a'..'h' (case insensitive).
     * \param rank Rank (row) of the square as a number 1..8.
     * \throws InvalidSquare if the file or rank is out of range.
     * \return A square with the given file and rank.
     */
    static Square from_coords(char file, int rank);

    char rank_name() const;
private:
    Square(int file, int rank);
    int m_file;
    int m_rank;
};

/**
 * \brief Representation of a chessboard.
 */
class Board {};

} // namespace chesscore

#endif
