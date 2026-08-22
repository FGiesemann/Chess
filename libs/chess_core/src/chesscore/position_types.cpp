/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/position_types.h"

namespace chesscore {

auto check_state_symbol(CheckState state) -> std::string {
    if (state == CheckState::Check) {
        return "+";
    }
    if (state == CheckState::Checkmate) {
        return "#";
    }
    return "";
}

auto PositionState::operator==(const PositionState &rhs) const -> bool {
    return side_to_move == rhs.side_to_move && fullmove_number == rhs.fullmove_number && halfmove_clock == rhs.halfmove_clock && castling_rights == rhs.castling_rights &&
           en_passant_target == rhs.en_passant_target;
}

} // namespace chesscore
