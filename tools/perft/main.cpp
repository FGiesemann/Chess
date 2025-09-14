/* ************************************************************************** *
 * Chess Tools - Perft                                                        *
 * Debugging and Tools for Chess                                              *
 * Performance Test (perft-Tool)                                              *
 * ************************************************************************** */

#include <iostream>
#include <ranges>
#include <span>
#include <string>
#include <vector>

#include <chesscore_io/chesscore_io.h>

#include "perft.h"

enum class Command { None, Perft, Divide };

auto to_string(Command command) -> std::string {
    switch (command) {
    case Command::None:
        return "None";
    case Command::Perft:
        return "Perft";
    case Command::Divide:
        return "Divide";
    }
    return "Unknown";
}

struct Options {
    Command command{Command::Perft};
    bool perform_range{false};
    std::string fen;
    int depth{0};
};

auto parse_arguments(const std::vector<std::string> &argv) -> Options;
auto print_help() -> void;
auto perform_perft(chesstools::perft::Position &pos, int depth, bool perform_range) -> void;
auto perform_divide(chesstools::perft::Position &pos, int depth, bool perform_range) -> void;
auto print_divide_result(const chesstools::perft::DivideResult &result) -> void;

auto main(int argc, char *argv[]) -> int {
    std::vector<std::string> arguments = {argv, argv + argc};
    const auto options = parse_arguments(std::vector<std::string>{argv, argv + argc});
    auto pos = chesstools::perft::Position{chesscore::FenString{options.fen}};
    if (options.command == Command::Perft) {
        perform_perft(pos, options.depth, options.perform_range);
    } else if (options.command == Command::Divide) {
        perform_divide(pos, options.depth, options.perform_range);
    }
    return 0;
}

auto perform_perft(chesstools::perft::Position &pos, int depth, bool perform_range) -> void {
    if (perform_range) {
        for (int perft_depth = 1; perft_depth <= depth; ++perft_depth) {
            const auto result = chesstools::perft::perft(pos, perft_depth);
            std::cout << perft_depth << ": " << result.node_count << " (" << result.duration.count() << " us)\n";
        }
    } else {
        const auto result = chesstools::perft::perft(pos, depth);
        std::cout << depth << ": " << result.node_count << " (" << result.duration.count() << " us)\n";
    }
}

auto perform_divide(chesstools::perft::Position &pos, int depth, bool perform_range) -> void {
    if (perform_range) {
        for (int perft_depth = 1; perft_depth <= depth; ++perft_depth) {
            const auto result = chesstools::perft::divide(pos, perft_depth);
            print_divide_result(result);
        }
    } else {
        const auto result = chesstools::perft::divide(pos, depth);
        print_divide_result(result);
    }
}

auto print_divide_result(const chesstools::perft::DivideResult &result) -> void {
    std::cout << "Divide result:\n";
    std::uint64_t total_nodes{0};
    for (const auto &entry : result.table) {
        std::cout << entry.move << ": " << entry.node_count << '\n';
        total_nodes += entry.node_count;
    }
    std::cout << "Duration: " << result.duration.count() << " us\n";
    std::cout << "Total nodes: " << total_nodes << '\n';
    std::cout << "Nodes per second: " << total_nodes / (result.duration.count() / 1000000.0) << '\n';
}

auto parse_arguments(const std::vector<std::string> &argv) -> Options {
    Options options{};
    int unnamed_arg_index = 0;
    for (int i = 1; i < argv.size(); ++i) {
        if (argv[i] == "-h" || argv[i] == "--help") {
            print_help();
            return {.command = Command::None};
        }
        if (argv[i] == "-c" || argv[i] == "--command") {
            if (i + 1 < argv.size()) {
                if (argv[i + 1] == "perft") {
                    options.command = Command::Perft;
                } else if (argv[i + 1] == "divide") {
                    options.command = Command::Divide;
                } else {
                    std::cerr << "Unknown command: " << argv[i + 1] << '\n';
                    print_help();
                    return {.command = Command::None};
                }
                ++i;
            }
        } else if (argv[i] == "-r" || argv[i] == "--range") {
            options.perform_range = true;
        } else {
            if (unnamed_arg_index == 0) {
                options.fen = argv[i];
            } else if (unnamed_arg_index == 1) {
                options.depth = std::stoi(argv[i]);
            } else {
                std::cerr << "Unexpected argument: " << argv[i] << '\n';
                print_help();
                return {.command = Command::None};
            }
            ++unnamed_arg_index;
        }
    }
    if (unnamed_arg_index != 2) {
        std::cerr << "Missing arguments\n";
        print_help();
        return {.command = Command::None};
    }
    return options;
}

auto print_help() -> void {
    std::cout << R"(perft - Performance Testing for Chess

Usage:
    perft [<options>...] <FEN> <depth>

Options:
    -c, --command <cmd>
        Command to execute. Availabe commands are
        - perft  : Perfoamance test (default)
        - divide : Divide test
    -r, --range
        Perform test over a range of depth starting with 1 and ending with the
        given depth.)";
}
