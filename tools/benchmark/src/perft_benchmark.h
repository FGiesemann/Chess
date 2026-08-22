/* ************************************************************************** *
 * Chess Tools - Benchmark                                                    *
 * Benchmarking tool for performance measurement                              *
 * ************************************************************************** */

#ifndef CHESS_TOOLS_BENCHMARK_PERFT_BENCHMARK_H
#define CHESS_TOOLS_BENCHMARK_PERFT_BENCHMARK_H

#include <filesystem>
#include <span>

#include <chesscore/epd.h>
#include <chesscore/position.h>

namespace benchmark::perft {

struct Options {
    std::filesystem::path epd_file;
    int iterations{5};
    std::optional<int> max_depth;
};

auto parse_perft_options(std::span<char *> argv) -> Options;
auto print_perft_help() -> void;
auto run_benchmark(const Options &options) -> void;

class Benchmark {
public:
    explicit Benchmark(const Options &options);
    auto run() -> void;
private:
    static constexpr int test_column_width = 80;
    chesscore::EpdSuite m_test_suite;
    int m_max_depth = 0;
    int m_iterations = 5;

    static auto warmup(chesscore::Position position) -> void;
    static auto measure_single_perft(chesscore::Position position, int depth) -> std::tuple<std::uint64_t, std::uint64_t, double>;

    static auto print_header() -> void;
    static auto print_result(const std::string &name, uint64_t nodes, double time) -> void;

    static auto median(std::vector<double> &values) -> double;
};

} // namespace benchmark::perft

#endif
