/* ************************************************************************** *
 * Chess Engine Maat                                                          *
 * Chess playing engine                                                       *
 * ************************************************************************** */

#ifndef CHESS_ENGINE_MAAT_LOGGER_H
#define CHESS_ENGINE_MAAT_LOGGER_H

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

namespace chess_engine {

class Logger {
public:
    Logger() = default;
    Logger(const Logger &) = delete;
    auto operator=(const Logger &) -> Logger & = delete;
    Logger(Logger &&) noexcept = default;
    auto operator=(Logger &&) noexcept -> Logger & = default;
    ~Logger() { disable(); }

    static auto instance() -> Logger & {
        static Logger logger;
        return logger;
    }

    auto indent() -> void { m_indent += 2; }
    auto unindent() -> void {
        m_indent -= 2;
        m_indent = std::max(m_indent, 0);
    }

    auto enable(const std::string &filepath, bool append = false) -> void {
        m_enabled = true;
        auto mode = append ? (std::ios::out | std::ios::app) : std::ios::out;
        m_file.open(filepath, mode);
        if (m_file.is_open()) {
            log_internal("SYS", "=== Engine logging started ===");
        }
    }

    auto disable() -> void {
        if (m_file.is_open()) {
            log_internal("SYS", "=== Engine logging stopped ===");
            m_file.close();
        }
        m_enabled = false;
    }

    [[nodiscard]] auto is_enabled() const -> bool { return m_enabled; }

    auto log_uci_in(const std::string &command) -> void {
        if (!m_enabled) {
            return;
        }
        log_internal("UCI<", command);
    }

    auto log_uci_out(const std::string &response) -> void {
        if (!m_enabled) {
            return;
        }
        log_internal("UCI>", response);
    }

    auto log_info(const std::string &message) -> void {
        if (!m_enabled) {
            return;
        }
        log_internal("INFO", message);
    }

    auto log_error(const std::string &message) -> void {
        if (!m_enabled) {
            return;
        }
        log_internal("ERR ", message);
    }

    auto log_debug(const std::string &message) -> void {
        if (!m_enabled) {
            return;
        }
        log_internal("DBG ", message);
    }

    auto log_search(const std::string &message) -> void {
        if (!m_enabled) {
            return;
        }
        log_internal("SRCH", message);
    }

    auto log_evaluation(const std::string &message) -> void {
        if (!m_enabled) {
            return;
        }
        log_internal("EVAL", message);
    }
private:
    bool m_enabled{false};
    std::ofstream m_file;
    int m_indent{0};

    auto log_internal(const std::string &tag, const std::string &message) -> void {
        if (!m_file.is_open()) {
            return;
        }

        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::floor<std::chrono::milliseconds>(now);

        m_file << std::format("{:%H:%M:%S} [{}] {:{}s}{:s}\n", time, tag, "", m_indent, message);
        m_file.flush();
    }
};

class LogStream {
public:
    explicit LogStream(void (Logger::*log_func)(const std::string &)) : m_log_func(log_func) {}

    template<typename T>
    auto operator<<(const T &value) -> LogStream & {
        m_stream << value;
        return *this;
    }

    ~LogStream() {
        if (Logger::instance().is_enabled()) {
            (Logger::instance().*m_log_func)(m_stream.str());
        }
    }
private:
    void (Logger::*m_log_func)(const std::string &);
    std::ostringstream m_stream;
};

inline auto log_indent() -> void {
    chess_engine::Logger::instance().indent();
}

inline auto log_unindent() -> void {
    chess_engine::Logger::instance().unindent();
}

inline auto log_uci_in(const std::string &in) -> void {
    chess_engine::Logger::instance().log_uci_in(in);
}
inline auto log_uci_out(const std::string &out) -> void {
    chess_engine::Logger::instance().log_uci_out(out);
}

inline auto log_info(const std::string &msg) -> void {
    chess_engine::Logger::instance().log_info(msg);
}

inline auto log_error(const std::string &msg) -> void {
    chess_engine::Logger::instance().log_error(msg);
}

inline auto log_debug(const std::string &msg) -> void {
    chess_engine::Logger::instance().log_debug(msg);
}

inline auto log_search(const std::string &msg) -> void {
    chess_engine::Logger::instance().log_search(msg);
}

inline auto log_evaluation(const std::string &msg) -> void {
    chess_engine::Logger::instance().log_evaluation(msg);
}

inline auto log_debug_stream() -> chess_engine::LogStream {
    return chess_engine::LogStream(&chess_engine::Logger::log_debug);
}

inline auto log_info_stream() -> chess_engine::LogStream {
    return chess_engine::LogStream(&chess_engine::Logger::log_info);
}

inline auto log_error_stream() -> chess_engine::LogStream {
    return chess_engine::LogStream(&chess_engine::Logger::log_error);
}

inline auto log_uci_in_stream() -> chess_engine::LogStream {
    return chess_engine::LogStream(&chess_engine::Logger::log_uci_in);
}

inline auto log_uci_out_stream() -> chess_engine::LogStream {
    return chess_engine::LogStream(&chess_engine::Logger::log_uci_out);
}

inline auto log_search_stream() -> chess_engine::LogStream {
    return chess_engine::LogStream(&chess_engine::Logger::log_search);
}

inline auto log_evaluation_stream() -> chess_engine::LogStream {
    return chess_engine::LogStream(&chess_engine::Logger::log_evaluation);
}

} // namespace chess_engine

#endif
