/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#include "chesscore/move.h"

#include <ranges>
#include <sstream>

namespace chesscore {

auto Move::operator==(const Move &rhs) const -> bool {
    return from == rhs.from && to == rhs.to && piece == rhs.piece && captured == rhs.captured && capturing_en_passant == rhs.capturing_en_passant && promoted == rhs.promoted;
}

auto to_string(const Move &move) -> std::string {
    std::stringstream sstr;
    if (move.piece.type != PieceType::Pawn) {
        sstr << move.piece.piece_char_colorless();
    }
    sstr << move.from.file().name() << move.from.rank().rank;
    if (move.captured) {
        sstr << 'x' << move.captured.value().piece_char_colorless();
    } else {
        sstr << "-";
    }
    sstr << move.to.file().name() << move.to.rank().rank;
    if (move.promoted) {
        sstr << "=" << move.promoted.value().piece_char_colorless();
    }
    if (move.capturing_en_passant) {
        sstr << " ep";
    }
    return sstr.str();
}

auto is_moving_same_piece(const Move &move1, const Move &move2) -> bool {
    return move1.from == move2.from && move1.to == move2.to && move1.piece == move2.piece && move1.captured == move2.captured &&
           move1.capturing_en_passant == move2.capturing_en_passant;
}

auto is_moving_same_piece_and_promotes(const Move &move1, const Move &move2, const Piece &promoted) -> bool {
    return is_moving_same_piece(move1, move2) && move1.promoted == promoted;
}

auto BasicMoveCompare::operator()(const Move &move1, const Move &move2) -> bool {
    return is_moving_same_piece(move1, move2);
}

auto PromotionMoveCompare::operator()(const Move &move1, const Move &move2) -> bool {
    return is_moving_same_piece_and_promotes(move1, move2, promoted);
}

auto move_list_contains_promotions(const MoveList &list, const Move &move) -> bool {
    const auto color = move.piece.color;
    return move_list_contains(
               list, move,
               PromotionMoveCompare{Piece{
                   .type = PieceType::Rook,
                   .color = color,
               }}
           ) &&
           move_list_contains(list, move, PromotionMoveCompare{Piece{.type = PieceType::Knight, .color = color}}) &&
           move_list_contains(list, move, PromotionMoveCompare{Piece{.type = PieceType::Bishop, .color = color}}) &&
           move_list_contains(list, move, PromotionMoveCompare{Piece{.type = PieceType::Queen, .color = color}});
}

auto to_string(const MoveList &moves) -> std::string {
    return std::views::transform(moves, [](const Move &move) -> std::string { return to_string(move); }) | std::views::join_with(std::string(", ")) |
           std::ranges::to<std::string>();
}

} // namespace chesscore
