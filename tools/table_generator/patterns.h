/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#ifndef CHESSCORE_TABLE_GEN_PATTERNS_H
#define CHESSCORE_TABLE_GEN_PATTERNS_H

#include "dimensions.h"

#include <bitset>

namespace chesscore::table_gen {

static constexpr std::size_t pattern_width{overhang + board_width};

using Pattern = std::bitset<pattern_width * pattern_width>;

extern const Pattern knight_pattern;
extern const Pattern bishop_pattern;
extern const Pattern rook_pattern;
extern const Pattern queen_pattern;
extern const Pattern king_pattern;

extern const Pattern north_ray_pattern;
extern const Pattern northeast_ray_pattern;
extern const Pattern east_ray_pattern;
extern const Pattern southeast_ray_pattern;
extern const Pattern south_ray_pattern;
extern const Pattern southwest_ray_pattern;
extern const Pattern west_ray_pattern;
extern const Pattern northwest_ray_pattern;

} // namespace chesscore::table_gen

#endif
