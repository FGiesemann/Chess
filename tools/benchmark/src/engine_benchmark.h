/* ************************************************************************** *
 * Chess Tools - Benchmark                                                    *
 * Benchmarking tool for performance measurement                              *
 * ************************************************************************** */

#ifndef CHESS_TOOLS_BENCHMARK_ENGINE_BENCHMARK_H
#define CHESS_TOOLS_BENCHMARK_ENGINE_BENCHMARK_H

#include <chrono>
#include <filesystem>
#include <span>

#include <chesscore/epd.h>
#include <chessengine/types.h>

namespace benchmark::engine {

struct Options {
    std::filesystem::path epd_file;
    chessengine::Depth::value_type depth{5};
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
    Options m_options;
    constexpr static int test_column_width = 80;

    struct SearchStats {
        chessengine::Depth::value_type depth{0};
        std::int64_t nodes{};
        std::chrono::milliseconds time{};
    };

    auto read_test_suite(const std::filesystem::path &epd_file) -> void;
    static auto print_header() -> void;
    auto benchmark_position(chesscore::Position position) const -> SearchStats;
    static auto print_result(const std::string &name, const SearchStats &stats) -> void;
};

} // namespace benchmark::engine

#endif
