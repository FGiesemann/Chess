/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_BOARD_H
#define CHESSCORE_BOARD_H

#include <array>
#include <vector>

#include "chesscore/chesscore.h"
#include "chesscore/move.h"
#include "chesscore/piece.h"
#include "chesscore/position_types.h"

namespace chesscore {

class Square;
enum class PieceType;
enum class Color;
class FenString;

/**
 * \brief Compass directions.
 *
 * The 8 compass directions in which sliding pieces can move.
 */
enum class RayDirection { North, NorthEast, East, SouthEast, South, SouthWest, West, NorthWest };

/**
 * \brief Number of different compass directions.
 */
static constexpr std::size_t ray_direction_count{8U};

/**
 * \brief List of all ray directions.
 *
 * Allows easily iterating over all directions.
 */
static constexpr std::array<RayDirection, ray_direction_count> all_ray_directions{RayDirection::North, RayDirection::NorthEast, RayDirection::East, RayDirection::SouthEast,
                                                                                  RayDirection::South, RayDirection::SouthWest, RayDirection::West, RayDirection::NorthWest};

/**
 * \brief Get the numeric index of a compass direction.
 *
 * The compass directions are enumerated clockwise, starting with north = 0,
 * north east = 1, ...
 * \param direction The compass direction.
 * \return The numerix index of the compass direction.
 */
inline auto get_index(const RayDirection &direction) -> std::size_t {
    return static_cast<std::size_t>(direction);
}

/**
 * \brief Allows testing for a ray direction.
 *
 * Tests, if the given ray direction is set in the given bitmask.
 * \param mask The bitmask.
 * \param direction The direction to test.
 * \return If the bitmask contains the direction.
 */
auto operator&(std::uint8_t mask, RayDirection direction) -> bool;

auto is_negative_direction(RayDirection direction) -> bool;

} // namespace chesscore

#endif
