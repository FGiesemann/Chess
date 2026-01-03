/* ************************************************************************** *
 * Chess Tools - Benchmark                                                    *
 * Benchmarking tool for performance measurement                              *
 * ************************************************************************** */

#include "benchmark.h"

#include <filesystem>
#include <iostream>

struct Options {
    std::filesystem::path epd_file;
    std::optional<int> max_depth;
};

auto print_usage() -> void {
    std::cout << R"(Usage: benchmark [options] <epd_file>
    
Options:
    -d, --depth <max_depth>  Maximum depth to search
)";
}

auto read_options(int argc, char *argv[]) -> Options {
    Options options;
    if (argc < 2) {
        print_usage();
        exit(1);
    }
    for (int i = 1; i < argc; ++i) {
        std::string arg{argv[i]};
        if (arg == "-d" || arg == "--depth") {
            if (i + 1 == argc) {
                print_usage();
                exit(1);
            }
            options.max_depth = std::stoi(argv[++i]);
        } else {
            std::filesystem::path epd_file{arg};
            if (!epd_file.is_absolute()) {
                epd_file = std::filesystem::current_path() / epd_file;
            }
            options.epd_file = epd_file;
        }
    }
    return options;
}

auto main(int argc, char *argv[]) -> int {
    const auto options = read_options(argc, argv);

    try {
        std::filesystem::path epd_file{options.epd_file};
        benchmark::ChessBenchmark benchmark{epd_file};
        if (options.max_depth) {
            benchmark.set_max_depth(*options.max_depth);
        }

        benchmark.run();
    } catch (const benchmark::BenchmarkError &error) {
        std::cerr << error.what() << std::endl;
        return 1;
    }

    return 0;
}
