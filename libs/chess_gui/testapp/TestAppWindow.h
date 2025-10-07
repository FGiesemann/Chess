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

    auto move_clicked(chessgame::Cursor cursor) -> void;
    auto move_double_clicked(chessgame::Cursor cursor) -> void;
    auto move_selected(chessgame::Cursor cursor) -> void;
private:
    void setupUi();

    std::shared_ptr<chessgame::Game> m_game = std::make_shared<chessgame::Game>();
    chessgame::Cursor m_mainline = m_game->edit();

    chessgui::ChessboardWidget *m_chessboard_widget;
    chessgui::ChessboardController *m_chessboard_controller;
    chessgui::MoveTreeModel *m_move_tree_model;
    chessgui::MoveTreeWidget *m_move_tree_view;
};

#endif
