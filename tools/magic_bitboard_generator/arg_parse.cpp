/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#include "arg_parse.h"
#include "magic_bitboard.h"

#include <iostream>

namespace magic_bitboard_generator {

auto print_usage() -> void {
    std::cout << R"(Magic Bitboard Generator
    
Usage: magic_bitboard_generator <scenario> [options]

Scenarios:
    check            Check given magic numbers
    search           Search for magic numbers
    generate <path>  Generate header files
    
Options:
    -h, --help       Print this help message
    -p, --piece      The type of the sliding piece
    -q, --square     The square on which the piece is placed
    -i, --iter       The number of iterations
    -m, --magic      The magic number
    -s, --shift      The shift
    -d, --database   The database file

Hints:
    For the search scenario, the piece type can be given as R, B or RB for both.
    Squares can be given by name (e. g. a1). Multiple squares can be given as a
    comma separated list or a range (e. g. a1-c4).
    A range of shifts can be searched by specifying a start and an end value
    (e. g. 52-54).

    The generate scenario needs a database file to generate the header files.
)";
}

auto convert_args(std::span<const char *> args) -> std::vector<std::string> {
    std::vector<std::string> result{};
    for (std::size_t i = 1; i < args.size(); ++i) {
        result.emplace_back(args[i]);
    }
    return result;
}

auto scenario(const std::string &name) -> Scenario {
    if (name == "check") {
        return Scenario::Check;
    }
    if (name == "search") {
        return Scenario::Search;
    }
    return Scenario::Unknown;
};

auto to_string(Scenario scenario) -> std::string {
    switch (scenario) {
    case Scenario::Check:
        return "check";
    case Scenario::Search:
        return "search";
    default:
        return "unknown";
    }
}

auto parse_arguments(const std::vector<std::string> &args) -> Args {
    Args result{};
    for (std::size_t i = 0; i < args.size(); ++i) {
        const auto &arg = args[i];
        const auto next_arg = i + 1 < args.size() ? args[i + 1] : "";

        if (arg == "-h" || arg == "--help") {
            print_usage();
            exit(0);
        } else if (arg == "check" || arg == "search") {
            result.scenario = scenario(arg);
        } else if (arg == "generate") {
            result.scenario = Scenario::Generate;
            result.output = next_arg;
            ++i;
        } else if (arg == "-p" || arg == "--piece") {
            result.piece_types = parse_piece_types(next_arg);
            ++i;
        } else if (arg == "-q" || arg == "--square") {
            result.squares = parse_squares(next_arg);
            ++i;
        } else if (arg == "-i" || arg == "--iter") {
            result.iterations = std::stoull(next_arg);
            ++i;
        } else if (arg == "-m" || arg == "--magic") {
            if (next_arg.length() > 1 && next_arg.starts_with("0x")) {
                result.magic_number = std::stoull(next_arg.substr(2), nullptr, 16);
            } else {
                result.magic_number = std::stoull(next_arg);
            }
            ++i;
        } else if (arg == "-s" || arg == "--shift") {
            result.shifts = parse_shifts(next_arg);
            ++i;
        } else if (arg == "-d" || arg == "--database") {
            result.database = next_arg;
            ++i;
        } else {
            throw ArgError("Unknown argument: " + arg);
        }
    }

    if (result.scenario == Scenario::Generate) {
        if (result.database.empty()) {
            throw ArgError("No database file specified");
        }
        if (result.output.empty()) {
            throw ArgError("No output file specified");
        }
    }
    return result;
}

auto parse_piece_types(const std::string &piece_types) -> std::vector<chesscore::PieceType> {
    std::vector<chesscore::PieceType> result{};
    for (const auto &piece_type : piece_types) {
        if (piece_type == 'R') {
            result.emplace_back(chesscore::PieceType::Rook);
        } else if (piece_type == 'B') {
            result.emplace_back(chesscore::PieceType::Bishop);
        } else {
            throw ArgError("Invalid piece type: " + std::string{piece_type});
        }
    }
    return result;
}

auto read_square(const std::string &squares, std::size_t index) -> std::pair<chesscore::Square, std::size_t> {
    const auto &c = squares[index];
    if (c >= 'a' && c <= 'h') {
        const char file = c;
        ++index;
        if (index >= squares.length()) {
            throw ArgError("Invalid square: " + squares);
        }
        const auto &c2 = squares[index];
        if (c2 >= '1' && c2 <= '8') {
            const char rank = c2;
            return std::make_pair(chesscore::Square{chesscore::File{file}, chesscore::Rank{rank - '1'}}, index + 1);
        }
    }
    throw ArgError("Invalid square: " + squares);
}

auto parse_squares(const std::string &squares) -> std::vector<chesscore::Square> {
    std::vector<chesscore::Square> result{};
    // squares can be a single square (a1), a list of squares (a1,b4,c5), a range (a1-c4) or a combination of those
    for (std::size_t i = 0; i < squares.length(); ++i) {
        const auto &c = squares[i];
        if (c == ',') {
            continue;
        }
        if (c == '-') {
            if (result.empty()) {
                throw ArgError("Invalid range of sqaures: " + squares);
            }
            const auto [end_square, index] = read_square(squares, i + 1);
            for (chesscore::Square square = result.back() + 1; square != end_square; square += 1) {
                result.emplace_back(square);
            }
            result.emplace_back(end_square);
            i = index - 1;
        } else {
            const auto [square, index] = read_square(squares, i);
            result.emplace_back(square);
            i = index - 1;
        }
    }
    return result;
}

auto read_shift(const std::string &shifts, std::size_t index) -> std::pair<std::uint64_t, std::size_t> {
    std::uint64_t result{};
    while (index < shifts.length()) {
        const auto &c = shifts[index];
        if (c >= '0' && c <= '9') {
            result = result * 10 + c - '0';
            ++index;
        } else {
            break;
        }
    }
    return std::make_pair(result, index);
}

auto parse_shifts(const std::string &shifts) -> std::vector<std::uint64_t> {
    std::vector<std::uint64_t> result{};
    for (std::size_t i = 0; i < shifts.length(); ++i) {
        const auto &c = shifts[i];
        if (c == ',') {
            continue;
        }
        if (c == '-') {
            if (result.empty()) {
                throw ArgError("Invalid range of shifts: " + shifts);
            }
            const auto [end_shift, index] = read_shift(shifts, i + 1);
            const auto start_shift = result.back();
            result.pop_back();
            const auto range = make_shift_range(start_shift, end_shift);
            result.insert(result.end(), range.begin(), range.end());
            i = index;
        } else {
            const auto [shift, index] = read_shift(shifts, i);
            result.emplace_back(shift);
            i = index - 1;
        }
    }
    return result;
}

} // namespace magic_bitboard_generator
