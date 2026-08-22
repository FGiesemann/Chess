/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_CHESSCORE_H
#define CHESSCORE_CHESSCORE_H

#include <exception>
#include <string>

namespace chesscore {

/**
 * \brief A general exception thrown by the chess core.
 */
class ChessException : public std::exception {
public:
    /**
     * \brief Construct a new exception object.
     *
     * \param message A message describing the error.
     */
    ChessException(std::string message) : m_message{std::move(message)} {}

    /**
     * \brief The message stored in the exception.
     *
     * \return The stored message.
     */
    [[nodiscard]] auto what() const noexcept -> const char * override { return m_message.c_str(); }
private:
    std::string m_message;
};

/**
 * \brief Exception thrown when an index is out of range.
 */
class OutOfRange : public ChessException {
public:
    /**
     * \brief Construct a new exception.
     *
     * \param message Description of the error.
     */
    OutOfRange(const std::string &message) : ChessException{message} {}
};

} // namespace chesscore

#endif
