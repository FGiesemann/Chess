/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/board.h"

#include <cctype>

namespace chesscore {

InvalidSquare::InvalidSquare(bool rank_invalid, bool file_invalid) : ChessException{"Invalid square coordinates"}, m_rank_invalid(rank_invalid), m_file_invalid(file_invalid) {}

Square Square::from_coords(int file, int rank) {
    return Square{file, rank};
}

Square Square::from_coords(char file, int rank) {
    bool invalid_file_lower = file < 'a' || file > 'h';
    bool invalid_file_upper = file < 'A' || file > 'H';
    if (invalid_file_lower && invalid_file_upper) {
        file = 'z'; // let the square constructor detect invalidity
    }
    return Square{std::tolower(file) - 'a' + 1, rank};
}

Square::Square(int file, int rank) : m_file(file), m_rank(rank) {
    bool file_invalid = m_file < 1 || m_file > 8;
    bool rank_invalid = m_rank < 1 || m_rank > 8;
    if (file_invalid || rank_invalid) {
        throw InvalidSquare{rank_invalid, file_invalid};
    }
}

char Square::rank_name() const {
    return static_cast<char>('a' + m_file - 1);
}

} // namespace chesscore
