/* ************************************************************************** *
 * Chess Engine Maat                                                          *
 * Chess playing engine                                                       *
 * ************************************************************************** */

#include "chess_engine/config.h"

namespace chess_engine {

auto Config::from_file([[maybe_unused]] const std::filesystem::path &filename) -> Config {
    return {};
}

} // namespace chess_engine
