/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#ifndef CHESSGUI_CHESSBOARD_ERROR_H
#define CHESSGUI_CHESSBOARD_ERROR_H

#include <QException>
#include <QString>

namespace chessgui {

class ChessboardError : public QException {
public:
    ChessboardError(const QString &message = "") : m_message{message} {}
    [[nodiscard]] auto what() const noexcept -> const char * override { return m_message.toStdString().c_str(); }

    auto raise() const -> void override { throw *this; }
    auto clone() const -> ChessboardError * override { return new ChessboardError{*this}; }
private:
    QString m_message;
};

} // namespace chessgui

#endif
