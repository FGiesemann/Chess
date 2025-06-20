/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/fen.h"

#include <array>
#include <cctype>

namespace chesscore {

FenString::FenString() : FenString{std::string{empty_fen}} {}

FenString::FenString(const std::string &fen_string) : m_fen_string{fen_string} {
    size_t pos = detail::check_piece_placement(fen_string);
    const auto side = detail::check_side_to_move(fen_string, pos);
    m_side_to_move = side.first;
    const auto castling = detail::check_castling_availability(fen_string, side.second);
    m_castling_availability = castling.first;
    const auto en_p = detail::check_en_passant_target_square(fen_string, m_side_to_move, castling.second);
    m_en_passant = en_p.first;
    const auto half = detail::check_halfmove_clock(fen_string, en_p.second);
    m_halfmove_clock = half.first;
    m_fullmove_number = detail::check_fullmove_number(fen_string, half.second);
}

auto FenString::starting_position() -> FenString {
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

auto check_piece_placement(const std::string &fen_string) -> std::size_t {
    PieceValidityChecker checker{fen_string};
    return checker.position();
}

auto check_side_to_move(const std::string &fen_string, std::size_t pos) -> std::pair<Color, std::size_t> {
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
    return std::make_pair(color == 'w' ? Color::White : Color::Black, pos + 2);
}

auto check_castling_availability(const std::string &fen_string, std::size_t pos) -> std::pair<CastlingAvailability, std::size_t> {
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
    CastlingAvailability ability{};
    for (size_t i = 0; i < castling_string.length(); ++i) {
        switch (castling_string[i]) {
        case 'K':
            ability.white_kingside = true;
            break;
        case 'Q':
            ability.white_queenside = true;
            break;
        case 'k':
            ability.black_kingside = true;
            break;
        case 'q':
            ability.black_queenside = true;
            break;
        default:
            break;
        }
    }
    return std::make_pair(ability, pos + castling_string.length() + 1);
}

auto check_en_passant_target_square(const std::string &fen_string, Color player_to_move, std::size_t pos) -> std::pair<std::optional<Square>, std::size_t> {
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
        return std::make_pair(std::nullopt, pos + 2);
    }
    const char file = fen_string[pos];
    if (file < 'a' || file > 'h') {
        throw InvalidFen{"Invalid en passant target square in FEN string"};
    }
    if (pos + 1 >= fen_string.length()) {
        throw InvalidFen{"Unexpected end of FEN string"};
    }
    const char rank = fen_string[pos + 1];
    if ((player_to_move == Color::White && rank != '6') || (player_to_move == Color::Black && rank != '3')) {
        throw InvalidFen{"Invalid en passant target square in FEN string"};
    }
    if (pos + 2 >= fen_string.length()) {
        throw InvalidFen{"Unexpected end of FEN string"};
    }
    if (fen_string[pos + 2] != ' ') {
        throw InvalidFen{"Invalid en passant target square in FEN string"};
    }
    return std::make_pair(Square{.file = File{file}, .rank = Rank{rank - '1' + 1}}, pos + 3);
}

auto check_halfmove_clock(const std::string &fen_string, std::size_t pos) -> std::pair<int, std::size_t> {
    if (pos >= fen_string.length()) {
        throw InvalidFen{"Unexpected end of FEN string"};
    }
    size_t start{pos};
    while (pos < fen_string.length() && fen_string[pos] != ' ') {
        if (std::isdigit(fen_string[pos]) == 0) {
            throw InvalidFen{"Invalid halfmove clock in FEN string"};
        }
        ++pos;
    }
    if (fen_string[pos] != ' ') {
        throw InvalidFen{"Invalid halfmove clock in FEN string"};
    }
    return std::make_pair(std::stoi(fen_string.substr(start, pos - start)), pos + 1);
}

auto check_fullmove_number(const std::string &fen_string, size_t pos) -> int {
    if (pos >= fen_string.length()) {
        throw InvalidFen{"Unexpected end of FEN string"};
    }
    size_t start{pos};
    while (pos < fen_string.length()) {
        if (std::isdigit(fen_string[pos]) == 0) {
            throw InvalidFen{"Invalid fullmove number in FEN string"};
        }
        ++pos;
    }
    return std::stoi(fen_string.substr(start, pos - start));
}

} // namespace detail

} // namespace chesscore
