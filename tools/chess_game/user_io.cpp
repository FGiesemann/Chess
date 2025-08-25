/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "user_io.h"
#include "commands.h"

#include <iostream>

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

auto handle_user_input(Context &context) -> void {
    std::string input{prompt_user()};

    if (input == "q" || input == "Q") {
        context.running = false;
    } else if (input == "h" || input == "H" || input == "?") {
        print_commands();
    } else if (input == "reset") {
        reset_game(context);
    } else if (input.starts_with("fen ")) {
        set_fen(input, context);
    } else if (parses_as_san(input, context)) {
        apply_san_move(input, context);
    } else if (input.starts_with("m? ")) {
        test_san_move(input.substr(3), context);
    } else if (input == "list") {
        list_legal_moves(context);
    } else {
        std::cout << "Input not recognized as a valid command. (Use h for help.)\n";
    }
}
