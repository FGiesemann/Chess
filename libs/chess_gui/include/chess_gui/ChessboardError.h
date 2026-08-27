/* ************************************************************************** *
 * Chess Gui                                                                  *
 * Graphical user interface components for chess games                        *
 * ************************************************************************** */

#ifndef CHESS_GUI_CHESSBOARD_ERROR_H
#define CHESS_GUI_CHESSBOARD_ERROR_H

#include <QException>
#include <QString>

namespace chess_gui {

class ChessboardError : public QException {
public:
    ChessboardError(const QString &message = "") : m_message{message} {}
    [[nodiscard]] auto what() const noexcept -> const char * override { return m_message.toStdString().c_str(); }
    [[nodiscard]] auto message() const noexcept -> const QString & { return m_message; }

    auto raise() const -> void override { throw *this; }
    auto clone() const -> ChessboardError * override { return new ChessboardError{*this}; }
private:
    QString m_message;
};

} // namespace chess_gui

#endif
