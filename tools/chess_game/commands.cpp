/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "commands.h"

#include <chesscore_io/chesscore_io.h>
#include <chessgame/move_matcher.h>
#include <iostream>

auto reset_game(Context &context) -> void {
    context.game = chessgame::Game{};
    context.mainline = context.game.cursor();
}

auto print_game_status(const Context &context) -> void {
    std::cout << '\n' << context.mainline.position();
}

auto set_fen(std::string &input, Context &context) -> void {
    std::vector<chessgame::metadata_tag> metadata{
        {.name = "FEN", .value = input.substr(4)}
    };
    context.game = chessgame::Game{metadata};
    context.mainline = context.game.cursor();
}

auto apply_san_move(const std::string &san_str, Context &context) -> void {
    const auto &position = context.mainline.position();
    const auto opt_san_move = chessgame::parse_san(san_str, position.side_to_move());
    if (!opt_san_move) {
        std::cout << "The SAN could not be parsed: " << to_string(opt_san_move.error().error_type) << '\n';
    }
    const auto &san_move = *opt_san_move;
    const auto legal_moves = position.all_legal_moves();
    const auto matched_moves = chessgame::match_san_move(san_move, legal_moves);

    if (matched_moves.empty()) {
        std::cout << "The move is not allowed in this position!\n";
    } else if (matched_moves.size() > 1) {
        std::cout << "The SAN move is ambiguous!\nPossible moves are:\n";
        for (const auto &move : matched_moves) {
            std::cout << "  " << move << '\n';
        }
    } else {
        context.mainline = context.mainline.play_move(matched_moves[0]);
    }
}

auto test_san_move(const std::string &san_str, Context &context) -> void {
    const auto &position = context.mainline.position();
    const auto opt_san_move = chessgame::parse_san(san_str, position.side_to_move());
    if (!opt_san_move) {
        std::cout << "The SAN could not be parsed: " << to_string(opt_san_move.error().error_type) << '\n';
    }
    const auto &san_move = *opt_san_move;
    const auto legal_moves = position.all_legal_moves();
    const auto matched_moves = chessgame::match_san_move(san_move, legal_moves);

    if (matched_moves.empty()) {
        std::cout << "The move is not allowed in this position!\n";
    } else if (matched_moves.size() > 1) {
        std::cout << "The SAN move is ambiguous!\nPossible moves are:\n";
        for (const auto &move : matched_moves) {
            std::cout << "  " << move << '\n';
        }
    } else {
        std::cout << "SAN move is legal: " << matched_moves[0] << '\n';
    }
}

auto list_legal_moves(Context &context) -> void {
    const auto legal_moves = context.mainline.position().all_legal_moves();
    for (const auto &move : legal_moves) {
        std::cout << move << '\n';
    }
}

auto parses_as_san(const std::string &str, const Context &context) -> bool {
    const auto san_move = chessgame::parse_san(str, context.mainline.position().side_to_move());
    return san_move.has_value();
}
