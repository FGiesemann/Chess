#include <fstream>
#include <iostream>

#include "chessgame/pgn.h"

int main(int argc, const char *argv[]) {
    if (argc < 2) {
        std::cout << "Provide path to PGN file\n";
        return 1;
    }

    std::ifstream pgn_file{argv[1]};
    if (!pgn_file.is_open()) {
        std::cout << "Unable to open PGN file\n";
        return 1;
    }

    chessgame::PGNParser parser{pgn_file};
    std::uint64_t count{0};
    bool parsing = true;
    bool need_to_skip = false;
    while (parsing) {
        try {
            if (need_to_skip) {
                std::cout << "... skipping to next game ...\n";
                parser.skip_to_next_game();
                need_to_skip = false;
                std::cout << "... skipping done ...\n";
            }
            ++count;
            const auto opt_game = parser.read_game();
            if (!opt_game.has_value()) {
                break;
            }
        } catch (const chessgame::PGNError &e) {
            std::cout << "(" << count << "): Error reading PGN file: " << to_string(e.type()) << " at line " << e.line() << ": " << e.what() << '\n';
            need_to_skip = true;
        } catch (const chesscore::InvalidFen &e) {
            std::cout << "(" << count << "): Error interpreting FEN: " << e.what() << '\n';
            need_to_skip = true;
        }
    }

    std::cout << "No more games.\n";
}
