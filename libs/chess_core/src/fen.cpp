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

auto invalid_piece_letter(char piece) -> bool {
    return piece != 'r' && piece != 'n' && piece != 'b' && piece != 'q' && piece != 'k' && piece != 'p' && piece != 'R' && piece != 'N' && piece != 'B' && piece != 'Q' && piece != 'K' && piece != 'P';
}

class PieceValidityChecker {
public:
    PieceValidityChecker(const std::string &fen_string) : length{fen_string.length()} {
        while (pos < length) {
            if (fen_string[pos] == ' ') {
                check_end_of_piece_placement();
                ++pos;
                return;
            }
            if (fen_string[pos] == '/') {
                switch_to_next_rank();
            } else if (std::isdigit(fen_string[pos]) != 0) {
                handle_digit(fen_string[pos]);
            } else {
                check_piece_letter(fen_string[pos]);
            }
            if (file > ranks) {
                throw InvalidFen{"Invalid FEN string: too many files"};
            }
            ++pos;
        }
    }

    [[nodiscard]] auto position() const -> size_t { return pos; }
private:
    static constexpr int ranks = 8;
    size_t pos{0};
    int rank{0};
    int file{0};
    bool number_last{false};
    size_t length{};

    void check_end_of_piece_placement() const {
        if (rank != ranks - 1) {
            throw InvalidFen{"Invalid FEN string: missing ranks"};
        }
        if (file != ranks) {
            throw InvalidFen{"Invalid FEN string: missing files"};
        }
    }

    void switch_to_next_rank() {
        if (file != ranks) {
            throw InvalidFen{"Invalid FEN string: missing files"};
        }
        ++rank;
        file = 0;
        number_last = false;
        if (rank >= ranks) {
            throw InvalidFen{"Too many ranks in FEN string"};
        }
    }

    void handle_digit(char digit) {
        if (number_last) {
            throw InvalidFen{"Invalid FEN string: two consecutive numbers"};
        }
        number_last = true;
        file += digit - '0';
    }

    void check_piece_letter(char piece) {
        if (invalid_piece_letter(piece)) {
            throw InvalidFen{"Invalid piece type in FEN string"};
        }
        ++file;
        number_last = false;
    }
};

auto check_piece_placement(const std::string &fen_string) -> size_t {
    PieceValidityChecker checker{fen_string};
    return checker.position();
}

auto check_side_to_move(const std::string &fen_string, size_t pos) -> size_t {
    if (pos + 1 > fen_string.length()) {
        throw InvalidFen{"Unexpected end of FEN string"};
    }
    const char color = fen_string[pos];
    if (color != 'w' && color != 'b') {
        throw InvalidFen{"Invalid side to move in FEN string"};
    }
    if (fen_string[pos + 1] != ' ') {
        throw InvalidFen{"Invalid side to move in FEN string"};
    }
    return pos + 2;
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
