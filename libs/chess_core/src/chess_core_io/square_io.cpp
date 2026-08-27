/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chess_core_io/square_io.h"

#include <iostream>

namespace chess_core {

auto operator<<(std::ostream &os, File file) -> std::ostream & {
    return os << "abcdefgh"[file.file - 1];
}

auto operator<<(std::ostream &os, Rank rank) -> std::ostream & {
    return os << rank.rank;
}

auto operator<<(std::ostream &os, Square square) -> std::ostream & {
    return os << square.file() << square.rank();
}

} // namespace chess_core
