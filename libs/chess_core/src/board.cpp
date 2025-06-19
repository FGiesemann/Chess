/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/board.h"

#include <cctype>

namespace chesscore {

File::File(char file) : file{static_cast<int>(std::tolower(file) - 'a') + 1} {
    if (file < 0 || file > 8) {
        throw OutOfRange{"File must be between 1 and 8"};
    }
}

File::File(int file) : file{file} {
    if (file < 1 || file > 8) {
        throw OutOfRange{"File must be between 1 and 8"};
    }
}

char File::name() const {
    return static_cast<char>(file + 'a' - 1);
}

Rank::Rank(int rank) : rank{rank} {
    if (rank < 1 || rank > 8) {
        throw OutOfRange{"Rank must be between 1 and 8"};
    }
}

} // namespace chesscore
