/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore_io/position_io.h"

namespace chesscore {

auto operator<<(std::ostream &os, const CastlingRights &rights) -> std::ostream & {
    bool found_one{false};
    if (rights['K']) {
        os << 'K';
        found_one = true;
    }
    if (rights['Q']) {
        os << 'Q';
        found_one = true;
    }
    if (rights['k']) {
        os << 'k';
        found_one = true;
    }
    if (rights['q']) {
        os << 'q';
        found_one = true;
    }
    if (!found_one) {
        os << '-';
    }
    return os;
}

} // namespace chesscore
