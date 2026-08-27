#ifndef TESTAPPWINDOW_H
#define TESTAPPWINDOW_H

#include <QMainWindow>

#include <chess_game/game.h>

#include "chess_gui/ChessboardController.h"
#include "chess_gui/ChessboardWidget.h"
#include "chess_gui/MoveTreeWidget.h"

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
    auto move_made(const chess_core::Move &move) -> void;

    auto move_clicked(chess_game::Cursor cursor) -> void;
    auto move_double_clicked(chess_game::Cursor cursor) -> void;
    auto move_selected(chess_game::Cursor cursor) -> void;
private:
    void setupUi();

    std::shared_ptr<chess_game::Game> m_game = std::make_shared<chess_game::Game>();
    chess_game::Cursor m_mainline = m_game->edit();

    chess_gui::ChessboardWidget *m_chessboard_widget;
    chess_gui::ChessboardController *m_chessboard_controller;
    chess_gui::MoveTreeModel *m_move_tree_model;
    chess_gui::MoveTreeWidget *m_move_tree_view;
};

#endif
