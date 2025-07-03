/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <iostream>

#include "bigmap.h"
#include "chesscore_io/bitmap_io.h"

auto main() -> int {
    using namespace chesscore::table_gen;
    Bigmap bigmap{knight_pattern};

    std::cout << bigmap << '\n';

    std::cout << bigmap.extract_board() << '\n';
    return 0;
}
