#include "TestAppWindow.h"

#include <QDebug>
#include <QLabel>
#include <QSizePolicy>
#include <QSplitter>
#include <QVBoxLayout>

TestAppWindow::TestAppWindow(QWidget *parent)
    : QMainWindow(parent), m_chessboard_widget(new chessgui::ChessboardWidget(this)), m_chessboard_controller(new chessgui::ChessboardController(m_chessboard_widget, this)),
      m_move_tree_model{new chessgui::MoveTreeModel(m_game, this)}, m_move_tree_view(new chessgui::MoveTreeWidget(this)) {
    m_move_tree_view->setupModel(m_move_tree_model);
    setupUi();

    connect(m_chessboard_controller, &chessgui::ChessboardController::move_made, this, &TestAppWindow::move_made);
}

auto TestAppWindow::move_made(const chesscore::Move &move) -> void {
    auto new_cursor = m_mainline.play_move(move);
    m_move_tree_model->onMoveAdded(m_mainline.node()->id(), 0);
    m_mainline = new_cursor;
}

void TestAppWindow::setupUi() {
    setWindowTitle(tr("Schach Layout Test (Board und Zugbaum)"));

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    // --- Linke Seite: Schachbrett (Flexibel) ---
    QWidget *leftPanel = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->addWidget(m_chessboard_widget);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // --- Rechte Seite: Zugbaum (Feste Breite) ---
    QWidget *rightPanel = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    QLabel *treeTitle = new QLabel(tr("Zugbaum"), rightPanel);
    treeTitle->setFont(QFont("Inter", 14, QFont::Bold));
    treeTitle->setAlignment(Qt::AlignCenter);
    rightLayout->addWidget(treeTitle);
    rightLayout->addWidget(m_move_tree_view);
    rightLayout->setContentsMargins(5, 5, 5, 5);

    const int fixedWidth = 350;
    rightPanel->setFixedWidth(fixedWidth);

    splitter->addWidget(leftPanel);
    splitter->addWidget(rightPanel);

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 0);

    setCentralWidget(splitter);
    resize(700 + fixedWidth, 700);
}
