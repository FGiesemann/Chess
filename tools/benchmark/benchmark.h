/* ************************************************************************** *
 * Chess Tools - Benchmark                                                    *
 * Benchmarking tool for performance measurement                              *
 * ************************************************************************** */

#ifndef CHESS_TOOLS_BENCHMARK_BENCHMARK_H
#define CHESS_TOOLS_BENCHMARK_BENCHMARK_H

#include "perft_benchmark.h"

#include <span>
#include <stdexcept>
#include <string>
#include <variant>

namespace benchmark {

enum class Command { 
    None, 
    Perft, 
};

using Options = std::variant<benchmark::perft::Options>;

struct Parameters {
    Command command{Command::None};
    Options options;
};

class BenchmarkError : public std::runtime_error {
public:
    explicit BenchmarkError(const std::string &message) : std::runtime_error(message) {}
};

auto print_usage() -> void;
auto read_options(int argc, std::span<char *> argv) -> Parameters;
auto run_benchmark(const Parameters &parameters) -> void;

} // namespace benchmark

#endif
