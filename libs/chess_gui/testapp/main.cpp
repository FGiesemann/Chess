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
        : QMainWindow(parent), m_board_widget{new ChessboardWidget(piece_folder, this)}, m_current_position{chesscore::FenString::starting_position()},
          m_selected_square(std::nullopt) {
        setWindowTitle("Schachbrett");
        setMinimumSize(400, 400);
        resize(600, 600);

        QWidget *central_widget = new QWidget(this);
        auto *layout = new QVBoxLayout(central_widget);
        layout->addWidget(m_board_widget);
        setCentralWidget(central_widget);

        m_board_widget->showPosition(m_current_position);

        connect(m_board_widget, &ChessboardWidget::mousePressed, this, &MainWindow::on_square_clicked);
        connect(m_board_widget, &ChessboardWidget::mouseReleased, this, &MainWindow::on_square_released);
    }
protected:
    auto keyPressEvent(QKeyEvent *event) -> void override {
        if (event->key() == Qt::Key_Escape) {
            cancel_move();
        }
    }
private slots:
    auto on_square_clicked(const chesscore::Square &square) -> void {
        if (!m_selected_square.has_value()) {
            const auto opt_piece = m_current_position.board().get_piece(square);
            if (opt_piece.has_value() && opt_piece.value().color == m_current_position.side_to_move()) {
                start_possible_move(square, opt_piece.value());
            }
        } else {
            if (square == m_selected_square.value()) {
                cancel_move();
            } else {
                try_move(square);
            }
        }
    }

    auto on_square_released(const chesscore::Square &square) -> void {
        if (m_selected_square.has_value() && square != m_selected_square.value()) {
            try_move(square);
        }
    }
private:
    ChessboardWidget *m_board_widget;
    chesscore::Position<chesscore::Bitboard> m_current_position;

    std::optional<chesscore::Square> m_selected_square;
    chesscore::MoveList m_legal_moves;

    auto start_possible_move(chesscore::Square square, chesscore::Piece piece) -> void {
        m_selected_square = square;
        m_board_widget->clearMarkedSquares();
        m_board_widget->setGhostPiece(piece, square);
        compute_piece_moves(square);
    }

    auto try_move(chesscore::Square square) -> void {
        const auto iter = std::ranges::find_if(m_legal_moves, [&](const chesscore::Move &move) { return move.to == square; });
        if (iter != m_legal_moves.end()) {
            perform_move(*iter);
        }
    }

    auto perform_move(const chesscore::Move &move) -> void {
        m_current_position.make_move(move);
        m_board_widget->showPosition(m_current_position);
        m_board_widget->clearMarkedSquares();
        m_board_widget->clearGhostPiece();
        m_selected_square = std::nullopt;
        m_legal_moves.clear();
    }

    auto cancel_move() -> void {
        m_board_widget->clearMarkedSquares();
        m_board_widget->clearGhostPiece();
        m_selected_square = std::nullopt;
        m_legal_moves.clear();
    }

    auto compute_piece_moves(chesscore::Square square) -> void {
        m_legal_moves.clear();
        const auto all_legal_moves = m_current_position.all_legal_moves();
        for (const auto &move : all_legal_moves) {
            if (move.from == square) {
                m_legal_moves.push_back(move);
                m_board_widget->markSquare(move.to);
            }
        }
    }
};

} // namespace chessgui

auto main(int argc, char *argv[]) -> int {
    QApplication a(argc, argv);
    chessgui::MainWindow window(QString{"D:/Programmierung/Projekte/Chess/ChessGui/data/pieces/alpha"});
    window.show();

    return a.exec();
}
#include "main.moc"
