/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#include "arg_parse.h"

#include "check_scenario.h"
#include "search_scenario.h"

#include <filesystem>
#include <iostream>

using namespace magic_bitboard_generator;

static const auto database_path = std::filesystem::path{"magic_bitboard_database.txt"};

auto main(int argc, const char *argv[]) -> int {
    try {
        const auto arguments = convert_args({argv, static_cast<std::size_t>(argc)});
        const auto args = parse_arguments(arguments);

        if (args.scenario == Scenario::Check) {
            check_scenario::check_magics(args);
        } else if (args.scenario == Scenario::Search) {
            search_scenario::search_magic(args);
        } else {
            std::cout << "Don't know what to do!\n";
        }

    } catch (const ArgError &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
