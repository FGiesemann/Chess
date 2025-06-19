/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#ifndef CHESSCORE_PIECE_H
#define CHESSCORE_PIECE_H

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

} // namespace chesscore

#endif
