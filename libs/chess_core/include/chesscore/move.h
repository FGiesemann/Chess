/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_MOVE_H
#define CHESSCORE_MOVE_H

#include <algorithm>
#include <optional>
#include <vector>

#include "chesscore/chesscore.h"
#include "chesscore/piece.h"
#include "chesscore/position_types.h"
#include "chesscore/square.h"

namespace chesscore {

/**
 * \brief Describes the move of a piece.
 *
 * The move describes the moving piece, but also contains information about the
 * game position before the move, so that the move can easily be reversed.
 */
struct Move {
    Square from;                      ///< From where the piece is moved.
    Square to;                        ///< To where the piece is moved.
    Piece piece;                      ///< Which piece is moving.
    std::optional<Piece> captured{};  ///< If the move captures a piece, this is the captured piece.
    bool capturing_en_passant{false}; ///< If this move is capturing en passant
    std::optional<Piece> promoted{};  ///< If the move promotes a piece, this is the promoted piece.

    CastlingRights castling_rights_before{};          ///< Castling rights before the move.
    int halfmove_clock_before{};                      ///< Halfmove clock before the move.
    std::optional<Square> en_passant_target_before{}; ///< En passant target square before the move.

    /**
     * \brief If the move is a castling move.
     *
     * A castling move is detected, when a King moves two squares to the side.
     * \return If this is a castling move.
     */
    auto is_castling() const -> bool { return piece.type == PieceType::King && from.rank() == to.rank() && std::abs(from.file().file - to.file().file) == 2; }

    /**
     * \brief If the move is a capture.
     *
     * This is a convenience function to check if the move is a capture.
     * \return If the move captures a piece.
     */
    auto is_capture() const -> bool { return captured.has_value(); }

    /**
     * \brief If this is a double-stop of a pawn.
     *
     * \return If the move is a double-step of a pawn.
     */
    auto is_double_step() const -> bool { return piece.type == PieceType::Pawn && std::abs(from.rank().rank - to.rank().rank) == 2 && from.file().file == to.file().file; }
};

/**
 * \brief Partial comparison of two moves.
 *
 * Two moves are considered equal by this function, if they
 *   - have the same starting and target squares,
 *   - move the same piece (type and color),
 *   - capture the same piece (type and color),
 *   - are both en passant or both not en passant,
 *   - are both castling moves or both not castling moves.
 * Promotions and changes in halfmove clock or castling rights are ignored.
 * \param move1 The first move to compare.
 * \param move2 The second move to compare.
 * \return If the moves are equal according to the criteria above.
 */
auto is_moving_same_piece(const Move &move1, const Move &move2) -> bool;

/**
 * \brief Function object to compare moves.
 *
 * Compares moves according to the is_moving_same_piece function.
 */
struct BasicMoveCompare {
    /**
     * \brief Comparison of two moves.
     *
     * Compares two moves according to the is_moving_same_piece function.
     * \param move1 The first move to compare.
     * \param move2 The second move to compare.
     * \return If the moves are equal according to the is_moving_same_piece function.
     */
    auto operator()(const Move &move1, const Move &move2) -> bool;
};

/**
 * \brief A list of moves.
 */
using MoveList = std::vector<Move>;

/**
 * \brief Checks if a move list contains a given move.
 *
 * For comparison of moves, a binary predicate can be given. By default, the
 * predicate is the is_moving_same_piece function.
 * \tparam BinaryPred Type of the equality check for moves.
 * \param list The list to check.
 * \param move The move to find in the list.
 * \param pred The predicate used to compare two moves.
 * \return If the list contains a move accurding to the comparison fucntion.
 */
template<typename BinaryPred = BasicMoveCompare>
auto move_list_contains(const MoveList &list, const Move &move, BinaryPred pred = BasicMoveCompare{}) -> bool {
    return std::any_of(list.begin(), list.end(), [&move, &pred](const Move &m) { return pred(m, move); });
}

} // namespace chesscore

#endif
