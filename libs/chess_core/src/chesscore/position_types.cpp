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

}
