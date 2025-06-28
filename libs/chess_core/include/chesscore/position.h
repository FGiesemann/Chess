/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_POSITION_H
#define CHESSCORE_POSITION_H

#include "chesscore/board.h"

namespace chesscore {

/**
 * \brief The current state of a chess game.
 *
 * Represents the state of a game of chess. It includes the current placement of
 * pieces on the board, the current turn, castling rights, the playerr to move,
 * and many other pieces of information that describe the state of the game.
 * \tparam BoardT The board representation that manages the pieces on the board.
 */
template<Board BoardT>
class Position {};

} // namespace chesscore

#endif
