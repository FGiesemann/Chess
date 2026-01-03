/* ************************************************************************** *
 * Chess Tools - Benchmark                                                    *
 * Benchmarking tool for performance measurement                              *
 * ************************************************************************** */

#ifndef CHESS_TOOLS_BENCHMARK_BENCHMARK_H
#define CHESS_TOOLS_BENCHMARK_BENCHMARK_H

#include <chesscore/epd.h>
#include <chesscore/position.h>

#include <filesystem>

namespace benchmark {

class BenchmarkError : public std::runtime_error {
public:
    explicit BenchmarkError(const std::string &message) : std::runtime_error(message) {}
};

class ChessBenchmark {
public:
    explicit ChessBenchmark(const std::filesystem::path &epd_file);
    auto set_max_depth(int depth) -> void { m_max_depth = depth; }

    auto run(int iterations = 5) -> void;
private:
    static constexpr int test_column_width = 80;
    chesscore::EpdSuite m_test_suite;
    int m_max_depth = 0;

    static auto warmup(chesscore::Position position) -> void;
    static auto measure_single_perft(chesscore::Position position, int depth)
        -> std::tuple<std::uint64_t, std::uint64_t, double>;

    static auto print_header() -> void;
    static auto print_result(const std::string &name, uint64_t nodes, double time) -> void;

    static auto median(std::vector<double> &values) -> double;
};

} // namespace benchmark

#endif
