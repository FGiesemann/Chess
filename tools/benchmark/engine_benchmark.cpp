/* ************************************************************************** *
 * Chess Tools - Benchmark                                                    *
 * Benchmarking tool for performance measurement                              *
 * ************************************************************************** */

#include <chessengine/types.h>
#include <ctime>
#include <fstream>
#include <iostream>

#include <chessengine/chess_engine.h>

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
                options.depth = static_cast<chessengine::Depth::value_type>(std::stoi(argv[i + 1]));
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

Benchmark::Benchmark(const Options &options) : m_options{options} {
    read_test_suite(options.epd_file);
}

auto Benchmark::run() -> void {
    std::cout << "Starting engine benchmark for " << m_test_suite.size() << " positions\n";
    print_header();

    std::int64_t grand_total_nodes = 0;
    std::chrono::milliseconds grand_total_time{};

    for (const auto &record : m_test_suite) {
        auto position = record.position;
        const auto fen_str = chesscore::FenString{position.piece_placement(), position.state()};
        auto stats = benchmark_position(position);

        grand_total_nodes += stats.nodes;
        grand_total_time += stats.time;

        print_result(fen_str.str(), stats);
    }

    double final_mnps = (static_cast<double>(grand_total_nodes) / static_cast<double>(grand_total_time.count())) / 1'000.0;
    std::cout << std::string(test_column_width + 50, '-') << '\n';
    std::cout << "Summary: " << std::fixed << std::setprecision(3) << final_mnps << " MNPS\n";
}

auto Benchmark::benchmark_position(chesscore::Position position) const -> SearchStats {
    chessengine::Config config{};
    config.search_config.iterative_deepening = m_options.iterative_deepning;
    chessengine::StopParameters stop_parameters{.max_search_depth = chessengine::Depth{m_options.depth}};

    chessengine::ChessEngine engine{config};
    engine.set_position(position);
    engine.search(stop_parameters);

    const auto search_stats = engine.search_stats();

    return SearchStats{.depth = search_stats.depth.value, .nodes = search_stats.nodes, .time = search_stats.elapsed_time};
}

auto Benchmark::read_test_suite(const std::filesystem::path &epd_file) -> void {
    m_test_suite = chesscore::EpdSuite{};
    std::ifstream file(epd_file);
    if (!file.is_open()) {
        throw BenchmarkError{"Unable to open EPD file: " + epd_file.string()};
    }
    m_test_suite = chesscore::read_epd(file);
}

auto Benchmark::print_result(const std::string &name, const SearchStats &stats) -> void {
    if (stats.time.count() > 0) {
        double mnps = (static_cast<double>(stats.nodes) / static_cast<double>(stats.time.count())) / 1'000.0;
        std::cout << std::left << std::setw(test_column_width) << name << std::right << std::setw(5) << stats.depth << std::setw(15) << stats.nodes << std::setw(15) << std::fixed
                  << std::setprecision(4) << stats.time.count() << std::setw(15) << std::fixed << std::setprecision(3) << mnps << '\n';
    }
}

auto Benchmark::print_header() -> void {
    std::cout << std::left << std::setw(test_column_width) << "Position" << std::right << std::setw(5) << "Depth" << std::setw(15) << "Total Nodes" << std::setw(15) << "Time (ms)" << std::setw(15)
              << "MNPS" << '\n';
    std::cout << std::string(test_column_width + 50, '-') << '\n';
}

} // namespace benchmark::engine
