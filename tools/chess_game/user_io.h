/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#ifndef CHESS_GAME_USER_IO_H
#define CHESS_GAME_USER_IO_H

#include "context.h"

auto print_game_status(const Context &context) -> void;
auto handle_user_input(Context &context) -> void;

#endif
