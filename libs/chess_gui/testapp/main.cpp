#include "chessgui/ChessboardWidget.h"
#include "chessgui/PieceSet.h"

#include <QApplication>
#include <QMainWindow>

#include <iostream>

chessgui::ChessboardWidget *chessboard;

auto squareClicked(const chesscore::Square &square) -> void {
    chessboard->markSquare(square);
}

auto main(int argc, char *argv[]) -> int {
    QApplication a(argc, argv);
    QMainWindow window;
    window.setWindowTitle("Chessboard App");
    window.setMinimumSize(400, 400);
    window.resize(600, 600);

    chessgui::PieceSet pieces{QString{"D:/Programmierung/Projekte/Chess/ChessGui/data/pieces/alpha"}};

    chessboard = new chessgui::ChessboardWidget(pieces, &window);
    chessboard->setPosition(chesscore::Position<chesscore::Bitboard>{chesscore::FenString::starting_position()});

    QObject::connect(chessboard, &chessgui::ChessboardWidget::squareClicked, squareClicked);

    window.setCentralWidget(chessboard);
    window.show();

    return a.exec();
}
