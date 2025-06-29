/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_MOVE_H
#define CHESSCORE_MOVE_H

#include <optional>

#include "chesscore/chesscore.h"
#include "chesscore/piece.h"
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

    CastlingRights castling_rights_before; ///< Castling rights before the move.
    size_t halfmove_clock_before;          ///< Halfmove clock before the move.

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

} // namespace chesscore

#endif
