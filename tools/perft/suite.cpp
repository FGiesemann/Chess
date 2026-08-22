/* ************************************************************************** *
 * Chess Tools - Perft                                                        *
 * Debugging and Tools for Chess                                              *
 * Performance Test (perft-Tool)                                              *
 * ************************************************************************** */

#include "suite.h"
#include "perft.h"

#include <chesscore/epd.h>

#include <fstream>
#include <iostream>
#include <sstream>

namespace chesstools::perft {

auto perform_perft_suite(const std::string &path, Reporter &reporter) -> void {
    int positions_count{0};
    int test_count{0};
    int pass_count{0};
    std::ifstream suite_input{path};
    if (!suite_input) {
        throw std::runtime_error{"Failed to open suite file: " + path};
    }
    const auto records = chesscore::read_epd(suite_input);
    for (const auto &record : records) {
        auto position = record.position;
        const auto fen_str = chesscore::FenString{position.piece_placement(), position.state()};
        reporter << fen_str.str() << '\n';
        positions_count += 1;
        const auto &test_cases = record.unknown_commands;
        for (const auto &test : test_cases) {
            const auto depth = std::stoi(test.operands[0]);
            const auto node_count = std::stoull(test.operands[1]);
            reporter << "  " << depth << ": ";
            const auto result = perft_count(position, depth);
            test_count += 1;
            if (result == node_count) {
                reporter << "OK\n";
                pass_count += 1;
            } else {
                reporter << "FAIL (" << result << " != " << node_count << ")\n";
                break;
            }
        }
        reporter << '\n';
    }

    reporter << "\nFinal result:\n";
    reporter << "Positions checked: " << positions_count << '\n';
    reporter << "Tests performed: " << test_count << '\n';
    reporter << "Tests passed: " << pass_count << '\n';
    if (pass_count == test_count) {
        reporter << "All tests passed!\n";
    } else {
        reporter << (test_count - pass_count) << " tests failed!\n";
    }
}

} // namespace chesstools::perft
