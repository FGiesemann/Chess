/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore_io/square_io.h"

#include <iostream>

namespace chesscore {

auto operator<<(std::ostream &os, const File &file) -> std::ostream & {
    return os << "abcdefgh"[file.file - 1];
}

auto operator<<(std::ostream &os, const Rank &rank) -> std::ostream & {
    return os << rank.rank;
}

auto operator<<(std::ostream &os, const Square &square) -> std::ostream & {
    return os << square.file() << square.rank();
}

} // namespace chesscore
