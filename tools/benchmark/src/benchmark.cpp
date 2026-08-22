/* ************************************************************************** *
 * Chess Tools - Benchmark                                                    *
 * Benchmarking tool for performance measurement                              *
 * ************************************************************************** */

#include <cstring>
#include <iostream>


#include "benchmark.h"

namespace benchmark {

auto print_usage() -> void {
    std::cout << R"(Usage: benchmark [command] [options]
    
Commands:
    perft  
        performance benchmark for low-level routines (e. g. move generation, make move, unmake move)

    engine
        benchmark computation performance of search algorithm on test positions

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

    if (std::strncmp(argv[1], "--help", 6) == 0 || std::strncmp(argv[1], "-h", 2) == 0) {
        print_usage();
        exit(0);
    }

    std::string command_str{argv[1]};

    if (command_str == "perft") {
        parameters.command = Command::Perft;
        parameters.options = perft::parse_perft_options(argv);
    } else if (command_str == "engine") {
        parameters.command = Command::Engine;
        parameters.options = engine::parse_engine_options(argv);
    } else {
        print_usage();
        exit(1);
    }

    return parameters;
}

auto run_benchmark(const Parameters &parameters) -> void {
    switch (parameters.command) {
    case Command::Perft:
        perft::run_benchmark(std::get<perft::Options>(parameters.options));
        break;
    case Command::Engine:
        engine::run_benchmark(std::get<engine::Options>(parameters.options));
        break;
    case Command::None:
        break;
    }
}

} // namespace benchmark
