/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/fen.h"

#include <algorithm>
#include <array>
#include <cctype>

namespace chesscore {

FenString::FenString() : FenString{std::string{empty_fen}} {}

FenString::FenString(const std::string &fen_string) : m_fen_string{fen_string} {
    const auto place = detail::check_piece_placement(fen_string);
    m_piece_placement = place.first;
    const auto side = detail::check_side_to_move(fen_string, place.second);
    m_side_to_move = side.first;
    const auto castling = detail::check_castling_availability(fen_string, side.second);
    m_castling_rights = castling.first;
    const auto en_p = detail::check_en_passant_target_square(fen_string, m_side_to_move, castling.second);
    m_en_passant = en_p.first;
    const auto half = detail::check_halfmove_clock(fen_string, en_p.second);
    m_halfmove_clock = half.first;
    m_fullmove_number = detail::check_fullmove_number(fen_string, half.second);
}

FenString::FenString(
    const PiecePlacement &piece_placement, Color side_to_move, const CastlingRights &castling_rights, std::optional<Square> en_passant, int halfmove_clock, int fullmove_number
)
    : m_piece_placement{piece_placement}, m_side_to_move{side_to_move}, m_castling_rights{castling_rights}, m_en_passant{en_passant}, m_halfmove_clock{halfmove_clock},
      m_fullmove_number{fullmove_number} {
    m_fen_string = detail::placement_to_string(m_piece_placement) + " " + (m_side_to_move == Color::White ? "w" : "b") + " ";
    if (m_castling_rights != CastlingRights::none()) {
        m_fen_string += detail::castling_rights_to_string(m_castling_rights);
    } else {
        m_fen_string += '-';
    }
    if (m_en_passant.has_value()) {
        m_fen_string += " " + to_string(m_en_passant.value());
    } else {
        m_fen_string += " -";
    }
    m_fen_string += " " + std::to_string(m_halfmove_clock) + " " + std::to_string(m_fullmove_number);
}

FenString::FenString(const PiecePlacement &placement, const PositionState &state)
    : FenString{placement, state.side_to_move, state.castling_rights, state.en_passant_target, state.halfmove_clock, state.fullmove_number} {}

auto FenString::starting_position() -> FenString {
    return FenString{std::string{starting_position_fen}};
}

namespace {

auto invalid_piece_letter(char piece) -> bool {
    return piece != 'r' && piece != 'n' && piece != 'b' && piece != 'q' && piece != 'k' && piece != 'p' && piece != 'R' && piece != 'N' && piece != 'B' && piece != 'Q' &&
           piece != 'K' && piece != 'P';
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
    [[nodiscard]] auto placement() const -> PiecePlacement { return piece_placement; }
private:
    static constexpr size_t ranks{8U};
    size_t pos{0};
    size_t rank{0U};
    size_t file{0U};
    bool number_last{false};
    size_t length{};
    PiecePlacement piece_placement;

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
        file += static_cast<size_t>(digit - '0');
    }

    void check_piece_letter(char piece) {
        if (invalid_piece_letter(piece)) {
            throw InvalidFen{"Invalid piece type in FEN string"};
        }
        if (file < ranks && rank < ranks) {
            std::size_t rank_offset{((ranks - 1) - rank) * ranks};
            piece_placement.at(rank_offset + file) = piece_from_fen_letter(piece);
        }
        ++file;
        number_last = false;
    }
};

} // namespace

namespace detail {

auto check_piece_placement(const std::string &fen_string) -> std::pair<PiecePlacement, std::size_t> {
    PieceValidityChecker checker{fen_string};
    return std::make_pair(checker.placement(), checker.position());
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

auto check_castling_availability(const std::string &fen_string, std::size_t pos) -> std::pair<CastlingRights, std::size_t> {
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
    CastlingRights ability{};
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
    return std::make_pair(Square{File{file}, Rank{rank - '1' + 1}}, pos + 3);
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

auto placement_to_string([[maybe_unused]] const PiecePlacement &placement) -> std::string {
    std::string result;
    int blank_count{0};
    for (int row = Rank::max_rank - 1; row >= 0; --row) {
        for (int column = 0; column < File::max_file; ++column) {
            int index = row * File::max_file + column;
            if (!placement[index].has_value()) {
                ++blank_count;
            } else {
                if (blank_count > 0) {
                    result += std::to_string(blank_count);
                    blank_count = 0;
                }
                result += placement[index].value().piece_char();
            }
        }
        if (blank_count > 0) {
            result += std::to_string(blank_count);
        }
        blank_count = 0;
        if (row > 0) {
            result += '/';
        }
    }
    return result;
}

auto castling_rights_to_string(const CastlingRights &castling_rights) -> std::string {
    std::string result;
    if (castling_rights.white_kingside) {
        result += 'K';
    }
    if (castling_rights.white_queenside) {
        result += 'Q';
    }
    if (castling_rights.black_kingside) {
        result += 'k';
    }
    if (castling_rights.black_queenside) {
        result += 'q';
    }
    if (result.empty()) {
        result += '-';
    }
    return result;
}

} // namespace detail

} // namespace chesscore
