/* ************************************************************************** *
 * Chess Engine Maat                                                          *
 * Chess playing engine                                                       *
 * ************************************************************************** */

#ifndef CHESS_ENGINE_MATE_IN_X_H
#define CHESS_ENGINE_MATE_IN_X_H

#include <chess_core/epd.h>
#include <chess_engine/config.h>
#include <chess_engine/logger.h>
#include <chess_engine/types.h>

#include <fstream>
#include <mutex>
#include <string>

namespace chess_engine::mate_in_x {

struct MateInXResult {
    bool found_mate{false};
    chess_engine::Depth expected_depth;
    chess_engine::Depth found_depth;
    chess_core::MoveList expected_moves;
    chess_core::Move found_move;
    chess_engine::SearchStats search_stats;
    std::string test_id;
};

class MateInXTest {
public:
    auto set_log(const std::string &log_file_path) -> void;
    auto set_threads(int thread_count) -> void { m_max_threads = thread_count; }
    auto set_config(const chess_engine::Config &config) -> void { m_base_config = config; }
    auto enable_debug() -> void { Logger::instance().enable("engine_debug.log"); }
    auto run_tests(const std::string &file_path, const std::string &first_test_id = "") -> void;

    auto test_count() const -> std::size_t { return m_tests.size(); }
    auto max_threads() const -> int { return m_max_threads; }
private:
    auto reset_stats() -> void;
    auto load_tests(const std::string &test_file_path) -> void;
    auto write_log(const std::string &message) -> void;
    auto calculate_places() -> void;
    auto process_tests(const std::string &first_test_id) -> void;
    auto perform_test(const chess_core::EpdRecord &test) -> MateInXResult;
    auto log_result(const MateInXResult &result) -> void;
    auto print_summary() -> void;

    chess_core::EpdSuite m_tests;
    std::ofstream m_log_file;
    int m_places;
    int m_tests_performed{0};
    int m_tests_passed{0};
    int m_max_threads{1};
    chess_engine::Config m_base_config;
    std::mutex m_log_mutex;
};

} // namespace chess_engine::mate_in_x

#endif
