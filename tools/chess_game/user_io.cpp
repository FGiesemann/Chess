/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "user_io.h"

#include <chesscore_io/chesscore_io.h>
#include <chessgame/move_matcher.h>
#include <chessgame/san.h>
#include <iostream>

auto print_game_status(const Context &context) -> void {
    std::cout << '\n' << context.mainline.position();
}

auto prompt_user(const std::string &prompt = "> ") -> std::string {
    std::string input{};
    std::cout << '\n' << prompt;
    std::getline(std::cin, input);
    std::cout << '\n';
    return input;
}

auto print_commands() -> void {
    std::cout << R"(Command overview:

Overall commands:
  h, H, ?
        Show this help
  q, Q    
        Quit the program

Game setup:
  reset
        Reset game to the starting position
  fen <FEN-String>
        Setup the board to the given FEN-String

Moves:
  <SAN>
        Play a move that is given in standard algebraic notation
  m? <SAN>
        Test, if the given SAN move is legal and unambiguous
  list
        List all legal moves
)";
}

auto handle_set_fen(std::string &input, Context &context) -> void {
    std::vector<chessgame::metadata_tag> metadata{
        {.name = "FEN", .value = input.substr(4)}
    };
    context.game = chessgame::Game{metadata};
    context.mainline = context.game.cursor();
}

auto handle_san_move(const chessgame::SANMove &san_move, Context &context) -> void {
    const auto &position = context.mainline.position();
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

auto handle_user_input(Context &context) -> void {
    std::string input{prompt_user()};

    if (input == "q" || input == "Q") {
        context.running = false;
    } else if (input == "h" || input == "H" || input == "?") {
        print_commands();
    } else if (input == "reset") {
        context.game = chessgame::Game{};
        context.mainline = context.game.cursor();
    } else if (input.starts_with("fen ")) {
        handle_set_fen(input, context);
    } else if (const auto san_move = chessgame::parse_san(input, context.mainline.position().side_to_move()); san_move.has_value()) {
        handle_san_move(san_move.value(), context);
    } else if (input.starts_with("m? ")) {
        test_san_move(input.substr(3), context);
    } else if (input == "list") {
        list_legal_moves(context);
    } else {
        std::cout << "Input not recognized as a valid command. (Use h for help.)\n";
    }
}
