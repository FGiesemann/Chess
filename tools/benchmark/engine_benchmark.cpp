/* ************************************************************************** *
 * Chess Tools - Benchmark                                                    *
 * Benchmarking tool for performance measurement                              *
 * ************************************************************************** */

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

auto Benchmark::run() -> void {}

auto Benchmark::read_test_suite(const std::filesystem::path &epd_file) -> void {
    m_test_suite = chesscore::EpdSuite{};
    std::ifstream file(epd_file);
    if (!file.is_open()) {
        throw BenchmarkError{"Unable to open EPD file: " + epd_file.string()};
    }
    m_test_suite = chesscore::read_epd(file);
}

} // namespace benchmark::engine
