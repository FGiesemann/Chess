#include "TestAppWindow.h"

#include <QDebug>
#include <QLabel>
#include <QSizePolicy>
#include <QSplitter>
#include <QVBoxLayout>

#include <chessgame/pgn.h>

TestAppWindow::TestAppWindow(QWidget *parent)
    : QMainWindow(parent), m_chessboard_widget(new chessgui::ChessboardWidget(this)), m_chessboard_controller(new chessgui::ChessboardController(m_chessboard_widget, this)),
      m_move_tree_model{new chessgui::MoveTreeModel(m_game, this)}, m_move_tree_view(new chessgui::MoveTreeWidget(this)) {
    m_move_tree_view->setupModel(m_move_tree_model);
    setupUi();

    connect(m_chessboard_controller, &chessgui::ChessboardController::move_made, this, &TestAppWindow::move_made);

    const std::string game_data = R"([Event "Test Event"]
[Site "Test Site"]
[White "Player W"]
[Black "Player B"]
[Result "1/2-1/2"]

1. d4 Nf6 (1... Nc6) 2. c4 (2. e4) 2... e6 (2... a5) 3. Nc3 Bb4 4. a3 Bxc3+ 5. bxc3 c5 6. f3 d5 7. e3 O-O
8. cxd5 Nxd5
9. Bd2 (9. c4 Ne7 10. Bd3 cxd4 11. exd4 Nf5 12. Bxf5 12... Qa5+)
9... Nc6 10. Bd3 cxd4 11. cxd4 e5
12. dxe5 (12. e4 Nf4 13. Bxf4 exf4 14. d5 Qh4+ 15. Kf1 15... Ne5 $36)
         (12. Ne2 12... exd4 13. exd4 Nxd4 14. Nxd4 Qh4+ 15. g3 Qxd4)
12... Nxe5
13. Be4 Nc4 $2 (13... Nf6 $1
    14. Bb4 (14. Bc3 Qc7 15. Qd4 Nxe4 16. fxe4 f6)
            (14. Bc2 Nd3+)
    14... Nxe4 15. Bxf8 Nd3+ 16. Kf1 Nef2 17. Qc2 17... Nxh1 $17)
14. Qc1 Nxd2 15. Qxd2 Nf6 16. Bd3 Re8 17. Ne2 Qb6
18. Nd4 Nd5 (18... Qxd4 $4 19. Bxh7+ Kxh7 20. Qxd4 $18)
19. Be4 Nxe3 1/2-1/2)";

    std::istringstream pgn_data{game_data};
    auto parser = chessgame::PGNParser{pgn_data};
    auto opt_game = parser.read_game();
    if (opt_game.has_value()) {
        m_move_tree_model->setGame(std::make_shared<chessgame::Game>(opt_game.value()));
        auto cursor = opt_game.value().current_mainline();
        m_chessboard_controller->set_position(cursor.position());
    }
}

auto TestAppWindow::move_made(const chesscore::Move &move) -> void {
    auto new_cursor = m_mainline.play_move(move);
    m_move_tree_model->onMoveAdded(m_mainline, m_mainline.child_count() - 1);
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
