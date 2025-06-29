/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_BOARD_H
#define CHESSCORE_BOARD_H

#include "chesscore/chesscore.h"
#include "chesscore/move.h"
#include "chesscore/piece.h"

namespace chesscore {

class Square;
enum class PieceType;
enum class Color;
class FenString;

/**
 * \brief A board describes the placement of pieces on a chess board.
 *
 */
template<typename T>
concept Board =
    requires(T board, const T board_c, const Square &square, const Piece &piece, const PieceType &piece_type, const Color &color, const Move &move, const FenString &fen) {
        std::is_default_constructible_v<T>;
        std::is_constructible_v<T, const FenString &>;
        { board_c.empty() } -> std::same_as<bool>;
        { board_c.has_piece(piece_type) } -> std::same_as<bool>;
        { board_c.has_piece(piece) } -> std::same_as<bool>;
        { board_c.has_piece(color) } -> std::same_as<bool>;
        { board_c.has_piece(square) } -> std::same_as<bool>;
        { board.set_piece(piece, square) } -> std::same_as<void>;
        { board_c.get_piece(square) } -> std::same_as<std::optional<Piece>>;
        { board.clear_square(square) } -> std::same_as<void>;
        { board.make_move(move) } -> std::same_as<void>;
        { board.unmake_move(move) } -> std::same_as<void>;
        // { board_c.calculate_hash_component() } -> std::same_as<uint64_t>; /* TODO: return type: Hash-Type */
    };

} // namespace chesscore

#endif
