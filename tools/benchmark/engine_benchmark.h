/* ************************************************************************** *
 * Chess Tools - Benchmark                                                    *
 * Benchmarking tool for performance measurement                              *
 * ************************************************************************** */

#ifndef CHESS_TOOLS_BENCHMARK_ENGINE_BENCHMARK_H
#define CHESS_TOOLS_BENCHMARK_ENGINE_BENCHMARK_H

#include <filesystem>
#include <span>

#include <chesscore/epd.h>

namespace benchmark::engine {

struct Options {
    std::filesystem::path epd_file;
    int depth{5};
    bool iterative_deepning{false};
};

auto parse_engine_options(std::span<char *> argv) -> Options;
auto print_engine_help() -> void;
auto run_benchmark(const Options &options) -> void;

class Benchmark {
public:
    explicit Benchmark(const Options &options);
    auto run() -> void;
private:
    chesscore::EpdSuite m_test_suite;

    auto read_test_suite(const std::filesystem::path &epd_file) -> void;
};

} // namespace benchmark::engine

#endif
