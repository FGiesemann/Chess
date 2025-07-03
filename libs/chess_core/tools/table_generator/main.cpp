/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <iostream>

#include "bigmap.h"

auto main() -> int {
    chesscore::table_gen::Bigmap bigmap{};
    std::cout << bigmap << '\n';
    return 0;
}
