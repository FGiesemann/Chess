#include "chessgui/ChessboardWidget.h"
#include "chessgui/PieceSet.h"

#include <QApplication>
#include <QKeyEvent>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>

#include <iostream>

namespace chessgui {

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(const QString &piece_folder, QWidget *parent = nullptr)
        : QMainWindow(parent), m_boardWidget{new ChessboardWidget(piece_folder, this)}, m_currentPosition{chesscore::FenString::starting_position()},
          m_selectedSquare(std::nullopt) {
        setWindowTitle("Schachbrett");
        setMinimumSize(400, 400);
        resize(600, 600);

        QWidget *centralWidget = new QWidget(this);
        auto *layout = new QVBoxLayout(centralWidget);
        layout->addWidget(m_boardWidget);
        setCentralWidget(centralWidget);

        m_boardWidget->setPosition(m_currentPosition);

        connect(m_boardWidget, &ChessboardWidget::squareClicked, this, &MainWindow::onSquareClicked);
    }
protected:
    auto keyPressEvent(QKeyEvent *event) -> void override {
        if (event->key() == Qt::Key_Escape) {
            if (m_selectedSquare.has_value()) {
                m_boardWidget->clearMarkedSquares();
                m_selectedSquare = std::nullopt;
                m_legal_moves.clear();
            }
        }
    }
private slots:
    auto onSquareClicked(const chesscore::Square &square) -> void {
        if (m_selectedSquare.has_value()) {
            chesscore::Square from_square = m_selectedSquare.value();
            qDebug() << "Wants to move from " << to_string(from_square) << " to " << to_string(square);

            auto iter = std::find_if(m_legal_moves.begin(), m_legal_moves.end(), [&](const chesscore::Move &move) { return move.to == square; });
            auto piece_at_target_square = m_currentPosition.board().get_piece(square);
            if (iter != m_legal_moves.end()) {
                m_currentPosition.make_move(*iter);
                m_boardWidget->setPosition(m_currentPosition);
                m_boardWidget->clearMarkedSquares();
                m_selectedSquare = std::nullopt;
                m_legal_moves.clear();
            } else if (piece_at_target_square.has_value()) {
                m_boardWidget->clearMarkedSquares();
                m_legal_moves.clear();
                m_selectedSquare = std::nullopt;
                const auto opt_piece = m_currentPosition.board().get_piece(square);
                if (opt_piece.has_value()) {
                    m_selectedSquare = square;
                    const auto all_legal_moves = m_currentPosition.all_legal_moves();
                    for (const auto &move : all_legal_moves) {
                        if (move.from == square) {
                            m_boardWidget->markSquare(move.to);
                            m_legal_moves.push_back(move);
                        }
                    }
                }
            } else {
                m_boardWidget->clearMarkedSquares();
                m_legal_moves.clear();
                m_selectedSquare = std::nullopt;
            }
        } else {
            m_boardWidget->clearMarkedSquares();
            m_legal_moves.clear();
            const auto opt_piece = m_currentPosition.board().get_piece(square);
            if (opt_piece.has_value() && opt_piece.value().color == m_currentPosition.side_to_move()) {
                m_selectedSquare = square;
                const auto all_legal_moves = m_currentPosition.all_legal_moves();
                for (const auto &move : all_legal_moves) {
                    if (move.from == square) {
                        m_boardWidget->markSquare(move.to);
                        m_legal_moves.push_back(move);
                    }
                }
            }
        }
    }
private:
    ChessboardWidget *m_boardWidget;
    chesscore::Position<chesscore::Bitboard> m_currentPosition;
    std::optional<chesscore::Square> m_selectedSquare;
    chesscore::MoveList m_legal_moves;
};

} // namespace chessgui

auto main(int argc, char *argv[]) -> int {
    QApplication a(argc, argv);
    chessgui::MainWindow window(QString{"D:/Programmierung/Projekte/Chess/ChessGui/data/pieces/alpha"});
    window.show();

    return a.exec();
}
#include "main.moc"
