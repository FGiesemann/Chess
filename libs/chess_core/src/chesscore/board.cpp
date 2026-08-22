/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/board.h"

namespace chesscore {

auto operator&(std::uint8_t mask, RayDirection direction) -> bool {
    return (mask & (1 << static_cast<std::underlying_type_t<RayDirection>>(direction))) != 0;
}

auto is_negative_direction(RayDirection direction) -> bool {
    return (direction == RayDirection::SouthEast || direction == RayDirection::South || direction == RayDirection::SouthWest || direction == RayDirection::West);
}

} // namespace chesscore
