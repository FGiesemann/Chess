/* ************************************************************************** *
 * Chess Tools - Perft                                                        *
 * Debugging and Tools for Chess                                              *
 * Performance Test (perft-Tool)                                              *
 * ************************************************************************** */

#include <iostream>
#include <span>
#include <string>

#include <chesscore_io/chesscore_io.h>

#include "perft.h"
#include "reporter.h"
#include "suite.h"

enum class Command { None, Perft, Divide };

auto get_command(std::string_view name) -> Command {
    if (name == "perft") {
        return Command::Perft;
    }
    if (name == "divide") {
        return Command::Divide;
    }
    return Command::None;
}

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

    static auto no_command() -> Options { return Options{.command = Command::None, .fen = "", .depth = 0, .suite_path = "", .report_path = ""}; }
};

auto parse_arguments(std::span<const char *> argv) -> Options;
auto print_help() -> void;
auto perform_perft(chesscore::Position &pos, int depth, bool perform_range, chesstools::perft::Reporter &reporter) -> void;
auto perform_divide(chesscore::Position &pos, int depth, bool perform_range, chesstools::perft::Reporter &reporter) -> void;
auto print_divide_result(const chesstools::perft::DivideResult &result, chesstools::perft::Reporter &reporter) -> void;

auto main(int argc, const char *argv[]) -> int {
    try {
        const auto options = parse_arguments(std::span<const char *>(argv, argc));
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
    reporter << "\nDuration: " << result.duration.count() << " us\n";
    reporter << "Total nodes: " << total_nodes << '\n';
    reporter << "Nodes per second: " << static_cast<double>(total_nodes) / (static_cast<double>(result.duration.count()) / 1000000.0) << '\n';
}

auto parse_arguments(std::span<const char *> argv) -> Options {
    using namespace std::string_literals;

    Options options{};
    int unnamed_arg_index = 0;
    for (size_t i = 1; i < argv.size(); ++i) {
        const std::string arg{argv[i]};

        if (arg == "-h"s || arg == "--help"s) {
            print_help();
            return Options::no_command();
        }

        if (arg == "-r"s || arg == "--range"s) {
            options.perform_range = true;
            continue;
        }

        if (arg[0] != '-') {
            std::cout << "Reading unnamed argument " << arg << '\n';
            if (unnamed_arg_index == 0) {
                options.fen = arg;
            } else if (unnamed_arg_index == 1) {
                options.depth = std::stoi(arg);
            } else {
                std::cerr << "Unexpected argument: " << arg << '\n';
                print_help();
                return Options::no_command();
            }
            ++unnamed_arg_index;
            continue;
        }

        if (i + 1 >= argv.size()) {
            std::cerr << "Option " << arg << " requires an argument\n";
            print_help();
            return Options::no_command();
        }
        const std::string option_arg{argv[i + 1]};

        if (arg == "-c"s || arg == "--command"s) {
            const auto command = get_command(option_arg);
            if (command == Command::None) {
                std::cerr << "Unknown command: " << option_arg << '\n';
                print_help();
                return Options::no_command();
            }
            options.command = command;
        } else if (arg == "-s"s || arg == "--suite"s) {
            options.suite_path = option_arg;
        } else if (arg == "-o"s || arg == "--output"s) {
            options.report_path = option_arg;
        }
        ++i;
    }
    if (options.suite_path.empty() && unnamed_arg_index != 2) {
        std::cerr << "Missing arguments\n";
        print_help();
        return Options::no_command();
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
        - perft  : Performance test (default)
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
