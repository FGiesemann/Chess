/* ************************************************************************** *
 * Chess Tools - Perft                                                        *
 * Debugging and Tools for Chess                                              *
 * Performance Test (perft-Tool)                                              *
 * ************************************************************************** */

#include "suite.h"
#include "perft.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace chesstools::perft {

namespace {

auto split_epd_fields(const std::string &line, const std::string &delimiter = ";") -> std::vector<std::string> {
    std::vector<std::string> fields;
    size_t index{0};
    size_t delim_pos = line.find(delimiter, index);
    while (delim_pos != std::string::npos) {
        size_t end = delim_pos - 1;
        while (end > 0 && line[end] == ' ') {
            --end;
        }
        fields.push_back(line.substr(index, end - index + 1));
        index = delim_pos + 1;
        delim_pos = line.find(delimiter, index);
    }
    if (index < line.size()) {
        fields.push_back(line.substr(index));
    }
    return fields;
}

} // namespace

auto perform_perft_suite(const std::string &path, Reporter &reporter) -> void {
    int positions_count{0};
    int test_count{0};
    int pass_count{0};
    const auto suite = parse_suite_definition(path);
    for (const auto &test : suite.tests) {
        reporter << test.fen.str() << '\n';
        Position position{test.fen};
        positions_count += 1;
        for (const auto &expected_result : test.expected_results) {
            reporter << "  " << expected_result.depth << ": ";
            const auto result = perft_count(position, expected_result.depth);
            test_count += 1;
            if (result == expected_result.node_count) {
                reporter << "OK\n";
                pass_count += 1;
            } else {
                reporter << "FAIL (" << result << " != " << expected_result.node_count << ")\n";
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

auto parse_suite_definition(const std::string &path) -> TestSuite {
    std::ifstream input_file{path};
    if (!input_file.is_open()) {
        std::cerr << "Failed to open file: " << path << '\n';
        return {};
    }

    TestSuite suite{};
    suite.name = path;
    std::string line;
    while (std::getline(input_file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        const auto fields = split_epd_fields(line);
        if (fields.size() < 2) {
            std::cerr << "Invalid EPD line: " << line << '\n';
            continue;
        }
        TestCase test_case{};
        test_case.fen = chesscore::FenString{fields[0]};
        for (size_t i = 1; i < fields.size(); ++i) {
            ExpectedResult expected_result{};
            const auto depth_count = split_epd_fields(fields[i], " ");
            if (depth_count.size() != 2) {
                std::cerr << "Invalid EPD line: " << line << '\n';
                continue;
            }
            expected_result.depth = std::stoi(depth_count[0].substr(1));
            expected_result.node_count = std::stoull(depth_count[1]);
            test_case.expected_results.push_back(expected_result);
        }
        suite.tests.push_back(test_case);
    }

    return suite;
}

} // namespace chesstools::perft
