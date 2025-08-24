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
    try {
        auto opt_game = parser.read_game();
        while (opt_game.has_value()) {
            ++count;
            std::cout << "Found PGN game " << count << ".\n";
            try {
                opt_game = parser.read_game();
            } catch (chessgame::PGNError &e) {
                std::cout << "PGN ERROR: (" << to_string(e.type()) << ") " << e.what() << " (line " << e.line() << ")\n";
                parser.skip_to_next_game();
            } catch (chesscore::InvalidFen &e) {
                std::cout << "ERROR: Cannot interpret FEN: " << e.what() << '\n';
                parser.skip_to_next_game();
            }
        }
    } catch (chessgame::PGNError &e) {
        std::cout << "PGN ERROR: (" << to_string(e.type()) << ") " << e.what() << " (line " << e.line() << ")\n";
        parser.skip_to_next_game();
    } catch (chesscore::InvalidFen &e) {
        std::cout << "ERROR: Cannot interpret FEN: " << e.what() << '\n';
        parser.skip_to_next_game();
    }
    std::cout << "No more games.\n";
}
