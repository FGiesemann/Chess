#include "chessgui/ChessboardWidget.h"
#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QMainWindow window;

    chessgui::ChessboardWidget *chessboard = new chessgui::ChessboardWidget(&window);
    window.setCentralWidget(chessboard);
    window.setWindowTitle("Chessboard App");
    window.resize(600, 600);
    window.show();

    return a.exec();
}
