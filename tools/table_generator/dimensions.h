/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#ifndef CHESSCORE_TABLE_GEN_DIMENSIONS_H
#define CHESSCORE_TABLE_GEN_DIMENSIONS_H

#include <cstddef>

namespace chesscore::table_gen {

static constexpr std::size_t board_width{8};
static constexpr std::size_t overhang{board_width - 1};
static constexpr std::size_t bigmap_width{2 * overhang + board_width};

} // namespace chesscore::table_gen

#endif
