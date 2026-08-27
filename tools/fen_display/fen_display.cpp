/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chess_core/bitboard.h"
#include "chess_core/fen.h"
#include "chess_core_io/bitboard_io.h"

#include <iostream>

auto main(int argc, const char *argv[]) -> int {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <FEN>\n";
        return 1;
    }

    const auto bitboard = chess_core::Bitboard{chess_core::FenString{argv[1]}};
    std::cout << bitboard;

    return 0;
}
