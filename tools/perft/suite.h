/* ************************************************************************** *
 * Chess Tools - Perft                                                        *
 * Debugging and Tools for Chess                                              *
 * Performance Test (perft-Tool)                                              *
 * ************************************************************************** */

#ifndef CHESS_TOOLS_PERFT_SUITE_H
#define CHESS_TOOLS_PERFT_SUITE_H

#include <string>
#include <vector>

#include <chesscore/fen.h>

#include "reporter.h"

namespace chesstools::perft {

struct ExpectedResult {
    int depth;
    std::uint64_t node_count;
};

struct TestCase {
    chesscore::FenString fen;
    std::vector<ExpectedResult> expected_results;
};

struct TestSuite {
    std::string name;
    std::vector<TestCase> tests;
};

auto parse_suite_definition(const std::string &path) -> TestSuite;
auto perform_perft_suite(const std::string &path, Reporter &reporter) -> void;

} // namespace chesstools::perft

#endif
