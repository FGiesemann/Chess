#include "chessgui/ChessboardWidget.h"
#include "chessgui/PieceSet.h"

#include <QApplication>
#include <QMainWindow>

#include <iostream>

chessgui::ChessboardWidget *chessboard;
chesscore::Position<chesscore::Bitboard> position;

auto squareClicked(const chesscore::Square &square) -> void {
    chessboard->clearMarkedSquares();
    const auto opt_piece = position.board().get_piece(square);
    if (opt_piece.has_value()) {
        const auto all_legal_moves = position.all_legal_moves();
        for (const auto &move : all_legal_moves) {
            if (move.from == square) {
                chessboard->markSquare(move.to);
            }
        }
    }
}

auto main(int argc, char *argv[]) -> int {
    QApplication a(argc, argv);
    QMainWindow window;
    window.setWindowTitle("Chessboard App");
    window.setMinimumSize(400, 400);
    window.resize(600, 600);

    position = chesscore::Position<chesscore::Bitboard>{chesscore::FenString::starting_position()};

    chessgui::PieceSet pieces{QString{"D:/Programmierung/Projekte/Chess/ChessGui/data/pieces/alpha"}};

    chessboard = new chessgui::ChessboardWidget(pieces, &window);
    chessboard->setPosition(position);

    QObject::connect(chessboard, &chessgui::ChessboardWidget::squareClicked, squareClicked);

    window.setCentralWidget(chessboard);
    window.show();

    return a.exec();
}
