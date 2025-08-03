/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/bitboard.h"
#include "chesscore/position.h"
#include "chesscore_io/chesscore_io.h"

#include <cstring>
#include <iostream>

auto main(int argc, const char *argv[]) -> int {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <FEN>\n";
        return 1;
    }
    bool constructor_output = false;
    if (argc == 3 && std::strncmp(argv[2], "-c", 2) == 0) {
        constructor_output = true;
    }

    const auto position = chesscore::Position<chesscore::Bitboard>{chesscore::FenString{argv[1]}};
    const auto moves = position.all_legal_moves();
    if (constructor_output) {
        for (const auto &move : moves) {
            write_move_constructor(std::cout, move);
            std::cout << '\n';
        }
    } else {
        std::cout << moves;
    }

    return 0;
}
