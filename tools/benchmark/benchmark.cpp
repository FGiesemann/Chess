/* ************************************************************************** *
 * Chess Tools - Benchmark                                                    *
 * Benchmarking tool for performance measurement                              *
 * ************************************************************************** */

#include <iostream>

#include "benchmark.h"

namespace benchmark {

auto print_usage() -> void {
    std::cout << R"(Usage: benchmark [command] [options]
    
Commands:
    perft  
        quick benchmark for low-level routines (e. g. move generation, make move, unmake move)


Options:
    --help, -h
        Display this help message and exit

    [command] --help | -h
        Display help for a specific command
)";
}

auto read_options(int argc, std::span<char *> argv) -> Parameters {
    if (argc < 2) {
        print_usage();
        exit(1);
    }

    Parameters parameters;

    if (strncmp(argv[1], "--help", 6) == 0 || strncmp(argv[1], "-h", 2) == 0) {
        print_usage();
        exit(0);
    }

    std::string command_str{argv[1]};

    if (command_str == "perft") {
        parameters.command = Command::Perft;
        parameters.options = benchmark::perft::parse_perft_options(argv);
    } else {
        print_usage();
        exit(1);
    }

    return parameters;
}

auto run_benchmark(const Parameters &parameters) -> void {
    switch (parameters.command) {
    case Command::Perft:
        benchmark::perft::run_benchmark(std::get<0>(parameters.options));
        break;
    case Command::None:
        break;
    }
}

} // namespace benchmark
