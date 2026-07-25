/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#ifndef MAGIC_BITBOARD_GENERATOR_ARG_PARSE_H
#define MAGIC_BITBOARD_GENERATOR_ARG_PARSE_H

#include <chesscore/piece.h>
#include <chesscore/square.h>

#include <span>
#include <stdexcept>
#include <string>
#include <vector>

namespace magic_bitboard_generator {

class ArgError : public std::runtime_error {
public:
    explicit ArgError(const std::string &message) : std::runtime_error(message) {}
};

enum class Scenario {
    Check,
    Search,

    Unknown
};

auto to_string(Scenario scenario) -> std::string;

auto scenario(const std::string &name) -> Scenario;

struct Args {
    Scenario scenario{Scenario::Unknown};
    std::vector<chesscore::PieceType> piece_types;
    std::vector<chesscore::Square> squares;
    std::uint64_t magic_number{};
    std::vector<std::uint64_t> shifts;
    std::string database{"magic_bitboard_database.txt"};
    std::uint64_t iterations{1'000'000};
};

auto print_usage() -> void;
auto convert_args(std::span<const char *> args) -> std::vector<std::string>;
auto parse_arguments(const std::vector<std::string> &args) -> Args;
auto parse_piece_types(const std::string &piece_types) -> std::vector<chesscore::PieceType>;
auto parse_squares(const std::string &squares) -> std::vector<chesscore::Square>;
auto parse_shifts(const std::string &shifts) -> std::vector<std::uint64_t>;

} // namespace magic_bitboard_generator

#endif
