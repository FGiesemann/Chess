/* ************************************************************************** *
 * Chess Tools - Benchmark                                                    *
 * Benchmarking tool for performance measurement                              *
 * ************************************************************************** */

#include <ctime>
#include <fstream>
#include <iostream>

#include "benchmark.h"
#include "engine_benchmark.h"

namespace benchmark::engine {

auto parse_engine_options(std::span<char *> argv) -> Options {
    Options options{.epd_file = "", .depth = 5, .iterative_deepning = false};

    for (size_t i = 1; i < argv.size(); ++i) {
        if (strncmp(argv[i], "--help", 6) == 0 || strncmp(argv[i], "-h", 2) == 0) {
            print_engine_help();
            exit(0);
        } else if (strncmp(argv[i], "--iterative_deepening", 21) == 0 || strncmp(argv[i], "-i", 2) == 0) {
            options.iterative_deepning = true;
        } else if (strncmp(argv[i], "--depth", 6) == 0 || strncmp(argv[i], "-d", 2) == 0) {
            if (i + 1 < argv.size()) {
                options.depth = std::stoi(argv[i + 1]);
                ++i;
            }
        } else {
            options.epd_file = argv[i];
        }
    }

    if (options.epd_file.empty()) {
        print_engine_help();
        exit(1);
    }

    return options;
}

auto print_engine_help() -> void {
    std::cout << R"(Usage: engine [options] epd_file
    
Options:
    --help, -h
        Display this help message and exit

    --depth, -d <depth>
        Search depth. (default = 5)

    --iterative_deepening, -i
        Use iterative deepening in search.

    [epd_file]
        Path to EPD file
)";
}

auto run_benchmark(const Options &options) -> void {
    Benchmark benchmark{options};
    benchmark.run();
}

Benchmark::Benchmark(const Options &options) {
    read_test_suite(options.epd_file);
}

auto Benchmark::run() -> void {
    std::cout << "Starting engine benchmark for " << m_test_suite.size() << " positions\n";
    print_header();

    uint64_t grand_total_nodes = 0;
    double grand_total_time = 0.0;

    for (const auto &record : m_test_suite) {
        auto position = record.position;
        const auto fen_str = chesscore::FenString{position.piece_placement(), position.state()};
        auto [depth, nodes, time] = benchmark_position(position);

        grand_total_nodes += nodes;
        grand_total_time += time;

        print_result(fen_str.str(), depth, nodes, time * 1000.0);
    }

    double final_mnps = (static_cast<double>(grand_total_nodes) / grand_total_time) / 1'000'000.0;
    std::cout << std::string(test_column_width + 50, '-') << '\n';
    std::cout << "Summary: " << std::fixed << std::setprecision(3) << final_mnps << " MNPS\n";
}

auto Benchmark::benchmark_position([[maybe_unused]] chesscore::Position position) -> std::tuple<int, std::uint64_t, double> {
    // TODO: Run search on position and return statistics
    return {5, 1548650ULL, 10.0};
}

auto Benchmark::read_test_suite(const std::filesystem::path &epd_file) -> void {
    m_test_suite = chesscore::EpdSuite{};
    std::ifstream file(epd_file);
    if (!file.is_open()) {
        throw BenchmarkError{"Unable to open EPD file: " + epd_file.string()};
    }
    m_test_suite = chesscore::read_epd(file);
}

auto Benchmark::print_result(const std::string &name, int depth, std::uint64_t nodes, double time) -> void {
    if (time > 0) {
        double mnps = (static_cast<double>(nodes) / time) / 1'000.0;
        std::cout << std::left << std::setw(test_column_width) << name << std::right << std::setw(5) << depth << std::setw(15) << nodes << std::setw(15) << std::fixed << std::setprecision(4) << time
                  << std::setw(15) << std::fixed << std::setprecision(3) << mnps << '\n';
    }
}

auto Benchmark::print_header() -> void {
    std::cout << std::left << std::setw(test_column_width) << "Position" << std::right << std::setw(5) << "Depth" << std::setw(15) << "Total Nodes" << std::setw(15) << "Time (ms)" << std::setw(15)
              << "MNPS" << '\n';
    std::cout << std::string(test_column_width + 50, '-') << '\n';
}

} // namespace benchmark::engine
