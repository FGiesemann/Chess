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

namespace chessgui {

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent), m_board_widget{new ChessboardWidget(this)}, m_board_controller{new ChessboardController(m_board_widget, this)} {
        setWindowTitle("Schachbrett");
        setMinimumSize(400, 400);
        resize(600, 600);

        auto *central_widget = new QWidget(this);
        auto *layout = new QVBoxLayout(central_widget);
        layout->addWidget(m_board_widget);
        setCentralWidget(central_widget);
    }
private:
    ChessboardWidget *m_board_widget;
    ChessboardController *m_board_controller;
};

} // namespace chessgui

auto main(int argc, char *argv[]) -> int {
    QApplication a(argc, argv);
    chessgui::MainWindow window;
    window.show();
    return a.exec();
}
#include "main.moc"
