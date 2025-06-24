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
concept Board = requires(T board, const T board_c, const Square &square, const Piece &piece, const PieceType &piece_type, const Color &color, /* const Move &move,*/ const FenString &fen) {
    { board_c.empty() } -> std::same_as<bool>;
    { board_c.has_piece(piece_type) } -> std::same_as<bool>;
    { board_c.has_piece(piece) } -> std::same_as<bool>;
    { board_c.has_piece(color) } -> std::same_as<bool>;
    { board_c.has_piece(square) } -> std::same_as<bool>;
    { board.set_piece(piece, square) } -> std::same_as<void>;
    { board_c.get_piece(square) } -> std::same_as<std::optional<Piece>>;
    { board.clear_square(square) } -> std::same_as<void>;
    { board.set_from_fen(fen) } -> std::same_as<void>;
    //{ board.make_move(move) } -> std::same_as<void>;
    //{ board.unmake_move(move) } -> std::same_as<void>;
    { board_c.calculate_hash_component() } -> std::same_as<uint64_t>; /* TODO: return type: Hash-Type */
};

} // namespace chesscore

#endif
