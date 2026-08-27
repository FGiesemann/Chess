/* ************************************************************************** *
 * Chess Engine Maat                                                          *
 * Chess playing engine                                                       *
 * ************************************************************************** */

#ifndef MATE_IN_X_TEST_MULTI_SOLUTION_FINDER_H
#define MATE_IN_X_TEST_MULTI_SOLUTION_FINDER_H

#include <chess_core/epd.h>
#include <chess_uci/gui_handler.h>

#include <condition_variable>
#include <mutex>
#include <string>

namespace chess_engine::mate_in_x {

class MultiSolutionFinder {
public:
    MultiSolutionFinder(const chess_uci::ProcessParams &params);
    ~MultiSolutionFinder();
    auto process(chess_core::EpdRecord &record) -> void;
private:
    enum class Callback { None, IsReady, BestMove };

    chess_uci::UCIGuiHandler m_uci_handler;
    chess_core::EpdSuite m_tests;
    std::mutex m_mutex;
    std::mutex m_info_mutex;
    std::condition_variable m_condvar;
    Callback m_received_callback{Callback::None};

    chess_core::EpdRecord *m_current_record{};

    auto readyok() -> void;
    auto search_info(const chess_uci::search_info &info) -> void;
    auto bestmove(const chess_uci::bestmove_info &info) -> void;
};

} // namespace chess_engine::mate_in_x

#endif
