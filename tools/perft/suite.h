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

auto perform_perft_suite(const std::string &path, Reporter &reporter) -> void;

} // namespace chesstools::perft

#endif
