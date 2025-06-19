/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/fen.h"

#include <cctype>

namespace chesscore {

FenString::FenString() : fen_string{empty_fen} {}

FenString::FenString(const std::string &fen_string) : fen_string{fen_string} {
    size_t pos = detail::check_piece_placement(fen_string);
    pos = detail::check_side_to_move(fen_string, pos);
    pos = detail::check_castling_availability(fen_string, pos);
    pos = detail::check_en_passant_target_square(fen_string, pos);
    pos = detail::check_halfmove_clock(fen_string, pos);
    detail::check_fullmove_number(fen_string, pos);
}

auto FenString::startingPosition() -> FenString {
    return FenString{std::string{starting_position_fen}};
}

namespace detail {

auto invalid_piece_letter(char c) -> bool {
    return c != 'r' && c != 'n' && c != 'b' && c != 'q' && c != 'k' && c != 'p' && c != 'R' && c != 'N' && c != 'B' && c != 'Q' && c != 'K' && c != 'P';
}

auto check_piece_placement(const std::string &fen_string) -> size_t {
    size_t pos{0};
    int rank{0};
    int file{0};
    bool number_last{false};
    while (pos < fen_string.length()) {
        if (fen_string[pos] == '/') {
            if (file != 8) {
                throw InvalidFen{"Invalid FEN string: missing files"};
            }
            ++rank;
            file = 0;
            number_last = false;
            if (rank > 7) {
                throw InvalidFen{"Too many ranks in FEN string"};
            }
        } else if (std::isdigit(fen_string[pos])) {
            if (number_last) {
                throw InvalidFen{"Invalid FEN string: two consecutive numbers"};
            }
            number_last = true;
            file += fen_string[pos] - '0';
        } else if (fen_string[pos] == ' ') {
            if (rank != 7) {
                throw InvalidFen{"Invalid FEN string: missing ranks"};
            } else if (file != 8) {
                throw InvalidFen{"Invalid FEN string: missing files"};
            } else {
                return pos + 1;
            }
        } else {
            if (invalid_piece_letter(fen_string[pos])) {
                throw InvalidFen{"Invalid piece type in FEN string"};
            }
            ++file;
            number_last = false;
        }
        if (file > 8) {
            throw InvalidFen{"Invalid FEN string: too many files"};
        }
        ++pos;
    }
    return pos;
}

auto check_side_to_move(const std::string &fen_string, size_t pos) -> size_t {
    if (pos >= fen_string.length()) {
        throw InvalidFen{"Unexpected end of FEN string"};
    }
    return pos;
}

auto check_castling_availability(const std::string &fen_string, size_t pos) -> size_t {
    if (pos >= fen_string.length()) {
        throw InvalidFen{"Unexpected end of FEN string"};
    }
    return pos;
}

auto check_en_passant_target_square(const std::string &fen_string, size_t pos) -> size_t {
    if (pos >= fen_string.length()) {
        throw InvalidFen{"Unexpected end of FEN string"};
    }
    return pos;
}

auto check_halfmove_clock(const std::string &fen_string, size_t pos) -> size_t {
    if (pos >= fen_string.length()) {
        throw InvalidFen{"Unexpected end of FEN string"};
    }
    return pos;
}

auto check_fullmove_number(const std::string &fen_string, size_t pos) -> void {
    if (pos >= fen_string.length()) {
        throw InvalidFen{"Unexpected end of FEN string"};
    }
}

} // namespace detail

} // namespace chesscore
