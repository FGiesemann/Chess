/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/fen.h"

#include <array>
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
    const auto next_space = fen_string.find(' ', pos);
    if (next_space == std::string::npos) {
        throw InvalidFen{"Unexpected end of FEN string"};
    }
    const auto castling_string = fen_string.substr(pos, next_space - pos);
    static const std::array<std::string, 16> valid_castlings{"KQkq", "KQk", "KQq", "KQ", "Kkq", "Kk", "Kq", "K", "Qkq", "Qk", "Qq", "Q", "kq", "k", "q", "-"};
    if (std::ranges::find(valid_castlings, castling_string) == std::end(valid_castlings)) {
        throw InvalidFen{"Invalid castling availability in FEN string"};
    }
    return pos + castling_string.length() + 1;
}

auto check_en_passant_target_square(const std::string &fen_string, size_t pos) -> size_t {
    if (pos >= fen_string.length()) {
        throw InvalidFen{"Unexpected end of FEN string"};
    }
    if (fen_string[pos] == '-') {
        if (pos + 1 >= fen_string.length()) {
            throw InvalidFen{"Unexpected end of FEN string"};
        }
        if (fen_string[pos + 1] != ' ') {
            throw InvalidFen{"Invalid en passant target square in FEN string"};
        }
        return pos + 2;
    }
    if (fen_string[pos] < 'a' || fen_string[pos] > 'h') {
        throw InvalidFen{"Invalid en passant target square in FEN string"};
    }
    if (pos + 1 >= fen_string.length()) {
        throw InvalidFen{"Unexpected end of FEN string"};
    }
    if (fen_string[pos + 1] != '3' && fen_string[pos + 1] != '6') {
        throw InvalidFen{"Invalid en passant target square in FEN string"};
    }
    if (pos + 2 >= fen_string.length()) {
        throw InvalidFen{"Unexpected end of FEN string"};
    }
    if (fen_string[pos + 2] != ' ') {
        throw InvalidFen{"Invalid en passant target square in FEN string"};
    }
    return pos + 3;
}

auto check_halfmove_clock(const std::string &fen_string, size_t pos) -> size_t {
    if (pos >= fen_string.length()) {
        throw InvalidFen{"Unexpected end of FEN string"};
    }
    while (pos < fen_string.length() && fen_string[pos] != ' ') {
        if (std::isdigit(fen_string[pos]) == 0) {
            throw InvalidFen{"Invalid halfmove clock in FEN string"};
        }
        ++pos;
    }
    if (fen_string[pos] != ' ') {
        throw InvalidFen{"Invalid halfmove clock in FEN string"};
    }
    return pos + 1;
}

auto check_fullmove_number(const std::string &fen_string, size_t pos) -> void {
    if (pos >= fen_string.length()) {
        throw InvalidFen{"Unexpected end of FEN string"};
    }
}

} // namespace detail

} // namespace chesscore
