/* ************************************************************************** *
 * Chess Tools - Benchmark                                                    *
 * Benchmarking tool for performance measurement                              *
 * ************************************************************************** */

#include "benchmark.h"

#include <chesscore/perft.h>

#include <fstream>
#include <iostream>
#include <numeric>
#include <string>

namespace benchmark {

ChessBenchmark::ChessBenchmark(const std::filesystem::path &epd_file) {
    m_test_suite = chesscore::EpdSuite{};
    std::ifstream file(epd_file);
    if (!file.is_open()) {
        throw BenchmarkError{"Unable to open EPD file: " + epd_file.string()};
    }
    m_test_suite = chesscore::read_epd(file);
}

auto ChessBenchmark::run(int iterations) -> void {
    std::cout << "Starting Benchmark (" << iterations << " iterations per position)" << std::endl;
    print_header();

    uint64_t grand_total_nodes = 0;
    double grand_total_time = 0.0;

    for (const auto &record : m_test_suite) {
        auto position = record.position;
        const auto fen_str = chesscore::FenString{position.piece_placement(), position.state()};

        warmup(position);

        for (const auto &test : record.unknown_commands) {
            const auto depth = std::stoi(test.operands[0]);
            if (depth >= m_max_depth) {
                continue;
            }

            std::vector<double> times;
            uint64_t nodes = 0;
            for (int i = 0; i < iterations; ++i) {
                auto [iter_nodes, iter_time] = measure_single_perft(position, depth);
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

auto ChessBenchmark::warmup(chesscore::Position position) -> void {
    chesscore::PerftCounter<chesscore::PerftMode::Benchmark> counter;
    chesscore::perftInternal<chesscore::PerftMode::Benchmark>(position, 3, counter);
}

auto ChessBenchmark::measure_single_perft(chesscore::Position position, int depth) -> std::pair<uint64_t, double> {
    auto start = std::chrono::high_resolution_clock::now();
    chesscore::PerftCounter<chesscore::PerftMode::Benchmark> counter;
    chesscore::perftInternal<chesscore::PerftMode::Benchmark>(position, depth, counter);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end - start;
    return {counter.totalNodes, diff.count()};
}

auto ChessBenchmark::print_header() -> void {
    std::cout << std::left << std::setw(test_column_width) << "Position" << std::right << std::setw(15) << "Total Nodes"
              << std::setw(15) << "Time (ms)" << std::setw(15) << "MNPS" << std::endl;
    std::cout << std::string(test_column_width + 45, '-') << std::endl;
}

auto ChessBenchmark::print_result(const std::string &name, std::uint64_t nodes, double time) -> void {
    double mnps = (nodes / time) / 1'000.0;
    std::cout << std::left << std::setw(test_column_width) << name << std::right << std::setw(15) << nodes
              << std::setw(15) << std::fixed << std::setprecision(4) << time << std::setw(15) << std::fixed
              << std::setprecision(3) << mnps << std::endl;
}

auto ChessBenchmark::median(std::vector<double> &values) -> double {
    std::sort(values.begin(), values.end());
    if (values.size() % 2 == 0) {
        return (values[values.size() / 2 - 1] + values[values.size() / 2]) / 2.0;
    }
    return values[values.size() / 2];
}

} // namespace benchmark
