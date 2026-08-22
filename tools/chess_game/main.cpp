/* ************************************************************************** *
 * Chess Tools                                                                *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "context.h"
#include "user_io.h"

auto main() -> int {
    Context context{};

    while (context.running) {
        print_game_status(context);
        handle_user_input(context);
    }

    return 0;
}
