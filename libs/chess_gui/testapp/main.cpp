#include "chessgui/ChessboardController.h"
#include "chessgui/ChessboardError.h"
#include "chessgui/ChessboardWidget.h"
#include "chessgui/PieceSet.h"

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
