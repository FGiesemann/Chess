/* ************************************************************************** *
 * Chess Tools - Benchmark                                                    *
 * Benchmarking tool for performance measurement                              *
 * ************************************************************************** */

#include "benchmark.h"

#include <iostream>
#include <span>

auto main(int argc, char *argv[]) -> int {
    const auto options = benchmark::read_options(argc, std::span<char *>(argv, argc));

    try {
        benchmark::run_benchmark(options);
    } catch (const benchmark::BenchmarkError &error) {
        std::cerr << error.what() << '\n';
        return 1;
    } catch (const chesscore::InvalidEpd &error) {
        std::cerr << error.what() << '\n';
        return 1;
    }

    return 0;
}
