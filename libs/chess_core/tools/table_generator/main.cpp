/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <iostream>
#include <unordered_map>

#include "bigmap.h"
#include "table_gen.h"

auto main(int argc, const char *argv[]) -> int {
    using namespace chesscore::table_gen;

    std::unordered_map<std::string, const Pattern &> patterns{
        {"knight", knight_pattern},
        {"bishop", bishop_pattern},
        {"rook",   rook_pattern  },
        {"queen",  queen_pattern },
        {"king",   king_pattern  },
    };

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <piece>\n";
        std::cerr << "  possible pieces:\n";
        for (const auto &e : patterns) {
            std::cerr << "    " << e.first << '\n';
        }
        return 1;
    }

    generate_table(argv[1], patterns.at(argv[1]), std::cout);

    return 0;
}
