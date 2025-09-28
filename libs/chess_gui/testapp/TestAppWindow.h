#ifndef TESTAPPWINDOW_H
#define TESTAPPWINDOW_H

#include <QMainWindow>

#include <chessgame/game.h>

#include "chessgui/ChessboardController.h"
#include "chessgui/ChessboardWidget.h"
#include "chessgui/MoveTreeWidget.h"

/**
 * @brief Hauptfenster zur reinen visuellen Anordnung von Schachbrett und Zugbaum.
 * Enthält temporär keine Logik (Controller-Teil), nur das Layout.
 */
class TestAppWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit TestAppWindow(QWidget *parent = nullptr);
    ~TestAppWindow() = default;
private slots:
    auto move_made(const chesscore::Move &move) -> void;
private:
    void setupUi();

    chessgame::Game m_game{};
    chessgame::Cursor m_mainline;

    chessgui::ChessboardWidget *m_chessboard_widget;
    chessgui::ChessboardController *m_chessboard_controller;
    chessgui::MoveTreeModel *m_move_tree_model;
    chessgui::MoveTreeWidget *m_move_tree_view;
};

#endif
