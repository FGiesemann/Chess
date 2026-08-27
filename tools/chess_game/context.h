/* ************************************************************************** *
 * Chess Tools                                                                *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#ifndef CHESS_GAME_CONTEXT_H
#define CHESS_GAME_CONTEXT_H

#include <chess_game/game.h>

struct Context {
    chess_game::Game game{};
    chess_game::Cursor mainline{game.cursor()};

    bool running{true};
};

#endif
