/* ************************************************************************** *
 * Chess Tools - Perft                                                        *
 * Debugging and Tools for Chess                                              *
 * Performance Test (perft-Tool)                                              *
 * ************************************************************************** */

#ifndef CHESS_TOOLS_PERFT_REPORTER_H
#define CHESS_TOOLS_PERFT_REPORTER_H

#include <fstream>
#include <iostream>
#include <string>

namespace chesstools::perft {

class Reporter {
public:
    Reporter() : m_write_report_file{false} {}

    explicit Reporter(const std::string &report_path) {
        if (!report_path.empty()) {
            m_write_report_file = true;
            m_report_file.open(report_path);
        }
    }

    template<typename T>
    auto operator<<(const T &t) -> Reporter & {
        std::cout << t;
        if (m_write_report_file) {
            m_report_file << t;
        }
        return *this;
    }
private:
    bool m_write_report_file;
    std::ofstream m_report_file{};
};

} // namespace chesstools::perft

#endif
