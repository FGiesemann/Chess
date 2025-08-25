/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#ifndef CHESS_GAME_COMMANDS_H
#define CHESS_GAME_COMMANDS_H

#include "context.h"
#include <chessgame/san.h>

auto reset_game(Context &context) -> void;
auto print_game_status(const Context &context) -> void;
auto set_fen(std::string &input, Context &context) -> void;
auto apply_san_move(const std::string &san_str, Context &context) -> void;
auto test_san_move(const std::string &san_str, Context &context) -> void;
auto list_legal_moves(Context &context) -> void;
auto parses_as_san(const std::string &str, const Context &context) -> bool;

#endif
