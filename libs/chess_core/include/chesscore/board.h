/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#ifndef CHESSCORE_BOARD_H
#define CHESSCORE_BOARD_H

#include "chesscore/chesscore.h"
#include "chesscore/fen.h"
#include "chesscore/piece.h"
#include "chesscore/square.h"

namespace chesscore {

/**
 * \brief A board describes the placement of pieces on a chess board.
 *
 */
template<typename T>
concept Board = requires(T board, Square s, Piece p, /* const Move &m,*/ const FenString &fen) {
    { board.set_from_fen(fen) } -> std::same_as<void>;
    //{ board.makeMove(m) } -> std::same_as<void>;
    //{ board.unmakeMove(m) } -> std::same_as<void>;
    { board.getPieceOnSquare(s) } -> std::same_as<Piece>;
    { board.setPieceOnSquare(s, p) } -> std::same_as<void>;
    { board.clearSquare(s) } -> std::same_as<void>;
    { board.calculateZobristHashComponent() } -> std::same_as<uint64_t>;
};

} // namespace chesscore

#endif
