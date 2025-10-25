/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "commands.h"

#include "chesscore_io/chesscore_io.h"
#include "chessgame/san.h"
#include <iostream>
#include <sstream>

auto reset_game(Context &context) -> void {
    context.game = chessgame::Game{};
    context.mainline = context.game.cursor();
}

auto print_game_status(const Context &context) -> void {
    std::cout << '\n' << context.mainline.position();
    std::cout << "White in check: " << context.mainline.position().is_king_in_check(chesscore::Color::White)
              << " | Black in check: " << context.mainline.position().is_king_in_check(chesscore::Color::Black) << '\n';
    const auto fen_str = chesscore::FenString{context.mainline.position().piece_placement(), context.mainline.position().state()};
    std::cout << "FEN: " << fen_str.str() << '\n';
}

auto set_fen(std::string &input, Context &context) -> void {
    chessgame::GameMetadata metadata;
    metadata.add("FEN", input.substr(4));
    context.game = chessgame::Game{metadata};
    context.mainline = context.game.cursor();
}

auto apply_san_move(const std::string &san_str, Context &context) -> bool {
    const auto &position = context.mainline.position();
    const auto opt_san_move = chessgame::parse_san(san_str, position.side_to_move());
    if (!opt_san_move) {
        std::cout << "The SAN could not be parsed: " << to_string(opt_san_move.error().error_type) << '\n';
        return false;
    }
    const auto &san_move = *opt_san_move;
    const auto legal_moves = position.all_legal_moves();
    const auto matched_moves = chessgame::match_move(san_move, legal_moves);

    if (matched_moves.empty()) {
        std::cout << "The move is not allowed in this position!\n";
        return false;
    }
    if (matched_moves.size() > 1) {
        std::cout << "The SAN move is ambiguous!\nPossible moves are:\n";
        for (const auto &move : matched_moves) {
            std::cout << "  " << move << '\n';
        }
        return false;
    }
    context.mainline = context.mainline.play_move(matched_moves[0]);
    return true;
}

auto test_san_move(const std::string &san_str, Context &context) -> void {
    const auto &position = context.mainline.position();
    const auto opt_san_move = chessgame::parse_san(san_str, position.side_to_move());
    if (!opt_san_move) {
        std::cout << "The SAN could not be parsed: " << to_string(opt_san_move.error().error_type) << '\n';
    }
    const auto &san_move = *opt_san_move;
    const auto legal_moves = position.all_legal_moves();
    const auto matched_moves = chessgame::match_move(san_move, legal_moves);

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

auto is_letter(char c) -> bool {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

auto play_moves(const std::string &movetext, Context &context) -> void {
    std::stringstream sstr{movetext};
    std::string token{};
    while (sstr >> token) {
        while (!token.empty() && !is_letter(token[0])) {
            token = token.substr(1);
        }
        if (token.empty()) {
            continue;
        }
        const auto opt_san_move = chessgame::parse_san(token, context.mainline.position().side_to_move());
        if (opt_san_move.has_value()) {
            std::cout << "\nPlaying move '" << token << "'\n";
            if (apply_san_move(token, context)) {
                print_game_status(context);
            } else {
                break;
            }
        }
    }
}
