/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#ifndef CHESSCORE_CHESSCORE_H
#define CHESSCORE_CHESSCORE_H

#include <exception>
#include <string>

namespace chesscore {

class ChessException : public std::exception {
public:
    ChessException(const std::string &message) : message{message} {};
    const char *what() const noexcept override { return message.c_str(); }
private:
    std::string message;
};

} // namespace chesscore

#endif
