/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/board.h"

#include <cctype>

namespace chesscore {

File::File(char file) : file{static_cast<int>(std::tolower(file) - 'a') + 1} {
    if (file < File::min_file || file > File::max_file) {
        throw OutOfRange{"File must be between 1 and 8"};
    }
}

File::File(int file) : file{file} {
    if (file < File::min_file || file > File::max_file) {
        throw OutOfRange{"File must be between 1 and 8"};
    }
}

auto File::name() const -> char {
    return static_cast<char>(file + 'a' - 1);
}

Rank::Rank(int rank) : rank{rank} {
    if (rank < Rank::min_rank || rank > Rank::max_rank) {
        throw OutOfRange{"Rank must be between 1 and 8"};
    }
}

} // namespace chesscore
