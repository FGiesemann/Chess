/* ************************************************************************** *
 * Chess Tools - Perft                                                        *
 * Debugging and Tools for Chess                                              *
 * Performance Test (perft-Tool)                                              *
 * ************************************************************************** */

#include <fstream>
#include <iostream>
#include <ranges>
#include <span>
#include <string>
#include <vector>

#include <chesscore_io/chesscore_io.h>

#include "perft.h"
#include "reporter.h"
#include "suite.h"

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
    std::string suite_path;
    std::string report_path;
};

auto parse_arguments(const std::vector<std::string> &argv) -> Options;
auto print_help() -> void;
auto perform_perft(chesscore::Position &pos, int depth, bool perform_range, chesstools::perft::Reporter &reporter) -> void;
auto perform_divide(chesscore::Position &pos, int depth, bool perform_range, chesstools::perft::Reporter &reporter) -> void;
auto print_divide_result(const chesstools::perft::DivideResult &result, chesstools::perft::Reporter &reporter) -> void;

auto main(int argc, char *argv[]) -> int {
    try {
        std::vector<std::string> arguments = {argv, argv + argc};
        const auto options = parse_arguments(std::vector<std::string>{argv, argv + argc});
        chesstools::perft::Reporter reporter{options.report_path};
        if (!options.suite_path.empty()) {
            chesstools::perft::perform_perft_suite(options.suite_path, reporter);
        } else if (options.command == Command::Perft) {
            auto pos = chesscore::Position{chesscore::FenString{options.fen}};
            perform_perft(pos, options.depth, options.perform_range, reporter);
        } else if (options.command == Command::Divide) {
            auto pos = chesscore::Position{chesscore::FenString{options.fen}};
            perform_divide(pos, options.depth, options.perform_range, reporter);
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}

auto perform_perft(chesscore::Position &pos, int depth, bool perform_range, chesstools::perft::Reporter &reporter) -> void {
    if (perform_range) {
        for (int perft_depth = 1; perft_depth <= depth; ++perft_depth) {
            const auto result = chesstools::perft::perft(pos, perft_depth);
            reporter << perft_depth << ": " << result.node_count << " (" << result.duration.count() << " us)\n";
        }
    } else {
        const auto result = chesstools::perft::perft(pos, depth);
        reporter << depth << ": " << result.node_count << " (" << result.duration.count() << " us)\n";
    }
}

auto perform_divide(chesscore::Position &pos, int depth, bool perform_range, chesstools::perft::Reporter &reporter) -> void {
    if (perform_range) {
        for (int perft_depth = 1; perft_depth <= depth; ++perft_depth) {
            const auto result = chesstools::perft::divide(pos, perft_depth);
            print_divide_result(result, reporter);
        }
    } else {
        const auto result = chesstools::perft::divide(pos, depth);
        print_divide_result(result, reporter);
    }
}

auto print_divide_result(const chesstools::perft::DivideResult &result, chesstools::perft::Reporter &reporter) -> void {
    reporter << "Divide result:\n";
    std::uint64_t total_nodes{0};
    for (const auto &entry : result.table) {
        reporter << entry.move << ": " << entry.node_count << '\n';
        total_nodes += entry.node_count;
    }
    reporter << "Duration: " << result.duration.count() << " us\n";
    reporter << "Total nodes: " << total_nodes << '\n';
    reporter << "Nodes per second: " << total_nodes / (result.duration.count() / 1000000.0) << '\n';
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
        } else if (argv[i] == "-s" || argv[i] == "--suite") {
            if (i + 1 < argv.size()) {
                options.suite_path = argv[i + 1];
                ++i;
            }
        } else if (argv[i] == "-o" || argv[i] == "--output") {
            if (i + 1 < argv.size()) {
                options.report_path = argv[i + 1];
                ++i;
            }
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
    if (options.suite_path.empty() && unnamed_arg_index != 2) {
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
        given depth.
    -s, --suite <path>
        Perform a perft test suite from the given path. The file must be in the
        EPD format
    -o, --output <path>
        Write output to specified file and the standard output)";
}
