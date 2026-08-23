#include "chess_gui/ChessboardController.h"
#include "chess_gui/ChessboardError.h"
#include "chess_gui/ChessboardWidget.h"
#include "chess_gui/PieceSet.h"

#include <QApplication>
#include <QKeyEvent>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QWidget>

#include <iostream>

#include "TestAppWindow.h"

auto main(int argc, char *argv[]) -> int {
    QApplication a(argc, argv);
    TestAppWindow window{};
    window.show();
    return a.exec();
}
