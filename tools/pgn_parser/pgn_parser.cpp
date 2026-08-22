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
                parser.skip_to_next_game();
                need_to_skip = false;
            }
            ++count;
            const auto opt_game = parser.read_game();
            if (!opt_game.has_value()) {
                break;
            }
            if (!parser.warnings().empty()) {
                std::cout << "(game " << count << "): Parsed with warnings:\n";
                for (const auto &warning : parser.warnings()) {
                    std::cout << "  (line " << warning.line << ") " << to_string(warning.type) << ": " << warning.description << '\n';
                }
            }
        } catch (const chessgame::PGNError &e) {
            std::cout << "(game " << count << "): Error reading PGN file: " << to_string(e.type()) << " at line " << e.line() << ": " << e.what() << '\n';
            need_to_skip = true;
        } catch (const chesscore::InvalidFen &e) {
            std::cout << "(game " << count << "): Error interpreting FEN: " << e.what() << '\n';
            need_to_skip = true;
        }
    }

    std::cout << "No more games.\n";
}
