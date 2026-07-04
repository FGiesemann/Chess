/* ************************************************************************** *
 * Chess Tools - Benchmark                                                    *
 * Benchmarking tool for performance measurement                              *
 * ************************************************************************** */

#include "perft_benchmark.h"
#include "benchmark.h"

#include <chesscore/perft.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

namespace benchmark::perft {

auto parse_perft_options(std::span<char *> argv) -> Options {
    Options options{.epd_file = "", .iterations = 5, .max_depth = std::nullopt};

    for (size_t i = 1; i < argv.size(); ++i) {
        if (strncmp(argv[i], "--help", 6) == 0 || strncmp(argv[i], "-h", 2) == 0) {
            print_perft_help();
            exit(0);
        } else if (strncmp(argv[i], "--iter", 6) == 0 || strncmp(argv[i], "-i", 2) == 0) {
            if (i + 1 < argv.size()) {
                options.iterations = std::stoi(argv[i + 1]);
                ++i;
            }
        } else if (strncmp(argv[i], "--depth", 6) == 0 || strncmp(argv[i], "-d", 2) == 0) {
            if (i + 1 < argv.size()) {
                options.max_depth = std::stoi(argv[i + 1]);
                ++i;
            }
        } else {
            options.epd_file = argv[i];
        }
    }

    if (options.epd_file.empty()) {
        print_perft_help();
        exit(1);
    }

    return options;
}

auto print_perft_help() -> void {
    std::cout << R"(Usage: perft [options] epd_file
    
Options:
    --help, -h
        Display this help message and exit

    --iter, -i <iterations>
        Number of iterations per position. (default = 5)

    --depth, -d <depth>
        Maximum depth to perft. If specified, all test cases with higher depths are skipped.

    [epd_file]
        Path to EPD file
)";
}

auto run_benchmark(const Options &options) -> void {
    Benchmark benchmark{options};
    benchmark.run();
}

Benchmark::Benchmark(const Options &options) {
    m_test_suite = chesscore::EpdSuite{};
    std::ifstream file(options.epd_file);
    if (!file.is_open()) {
        throw BenchmarkError{"Unable to open EPD file: " + options.epd_file.string()};
    }
    m_test_suite = chesscore::read_epd(file);
    m_iterations = options.iterations;
    m_max_depth = options.max_depth.has_value() ? *options.max_depth : 0;
}

auto Benchmark::run() -> void {
    std::cout << "Starting perft benchmark (" << m_iterations << " iterations per position) for " << m_test_suite.size() << " positions\n";
    print_header();

    uint64_t grand_total_nodes = 0;
    double grand_total_time = 0.0;

    for (const auto &record : m_test_suite) {
        auto position = record.position;
        const auto fen_str = chesscore::FenString{position.piece_placement(), position.state()};

        warmup(position);

        for (const auto &test : record.unknown_commands) {
            const auto depth = std::stoi(test.operands[0]);
            const auto reference_node_count = std::stoull(test.operands[1]);
            if (m_max_depth > 0 && depth > m_max_depth) {
                continue;
            }

            std::vector<double> times;
            uint64_t nodes = 0;
            for (int i = 0; i < m_iterations; ++i) {
                auto [leaf_nodes, iter_nodes, iter_time] = measure_single_perft(position, depth);
                if (leaf_nodes != reference_node_count) {
                    std::cerr << "ERROR: perft result " << leaf_nodes << " does not match expected count " << reference_node_count << '\n';
                }
                nodes = iter_nodes;
                times.push_back(iter_time);
            }

            const auto median_time = median(times);

            print_result(fen_str.str() + " @ " + std::to_string(depth), nodes, median_time * 1000.0);

            grand_total_nodes += nodes;
            grand_total_time += median_time;
        }
    }

    double final_mnps = (static_cast<double>(grand_total_nodes) / grand_total_time) / 1'000'000.0;
    std::cout << std::string(test_column_width + 45, '-') << '\n';
    std::cout << "Summary: " << std::fixed << std::setprecision(3) << final_mnps << " MNPS (Average of Medians)\n";
}

auto Benchmark::warmup(chesscore::Position position) -> void {
    chesscore::PerftCounter<chesscore::PerftMode::Benchmark> counter;
    chesscore::perft<chesscore::PerftMode::Benchmark>(position, 3, counter);
}

auto Benchmark::measure_single_perft(chesscore::Position position, int depth) -> std::tuple<std::uint64_t, std::uint64_t, double> {
    auto start = std::chrono::high_resolution_clock::now();
    chesscore::PerftCounter<chesscore::PerftMode::Benchmark> counter;
    chesscore::perft<chesscore::PerftMode::Benchmark>(position, depth, counter);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end - start;
    return {counter.leaf_nodes, counter.total_nodes, diff.count()};
}

auto Benchmark::print_header() -> void {
    std::cout << std::left << std::setw(test_column_width) << "Position" << std::right << std::setw(15) << "Total Nodes" << std::setw(15) << "Time (ms)" << std::setw(15) << "MNPS" << '\n';
    std::cout << std::string(test_column_width + 45, '-') << '\n';
}

auto Benchmark::print_result(const std::string &name, std::uint64_t nodes, double time) -> void {
    double mnps = (static_cast<double>(nodes) / time) / 1'000.0;
    std::cout << std::left << std::setw(test_column_width) << name << std::right << std::setw(15) << nodes << std::setw(15) << std::fixed << std::setprecision(4) << time << std::setw(15) << std::fixed
              << std::setprecision(3) << mnps << '\n';
}

auto Benchmark::median(std::vector<double> &values) -> double {
    std::ranges::sort(values);
    if (values.size() % 2 == 0) {
        return (values[values.size() / 2 - 1] + values[values.size() / 2]) / 2.0;
    }
    return values[values.size() / 2];
}

} // namespace benchmark::perft
