/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#ifndef CHESSGUI_CHESSBOARD_ERROR_H
#define CHESSGUI_CHESSBOARD_ERROR_H

#include <exception>
#include <string>

namespace chessgui {

class ChessboardError : public std::exception {
public:
    ChessboardError(std::string message) : m_message{std::move(message)} {}
    [[nodiscard]] auto what() const noexcept -> const char * override { return m_message.c_str(); }
private:
    std::string m_message;
};

} // namespace chessgui

#endif
