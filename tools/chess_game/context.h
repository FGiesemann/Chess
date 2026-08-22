/* ************************************************************************** *
 * Chess Tools                                                                *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#ifndef CHESS_GAME_CONTEXT_H
#define CHESS_GAME_CONTEXT_H

#include <chessgame/game.h>

struct Context {
    chessgame::Game game{};
    chessgame::Cursor mainline{game.cursor()};

    bool running{true};
};

#endif
