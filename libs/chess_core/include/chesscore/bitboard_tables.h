/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_BITBOARD_TABLES_H
#define CHESSCORE_BITBOARD_TABLES_H

#include "chesscore/bitmap.h"
#include "chesscore/board.h"
#include "chesscore/piece.h"
#include "chesscore/square.h"
#include "chesscore/table.h"

namespace chesscore::bitmaps {

using TargetTable = Table<Bitmap, 64, Square>;

extern const TargetTable knight_target_table;
extern const TargetTable bishop_target_table;
extern const TargetTable rook_target_table;
extern const TargetTable queen_target_table;
extern const TargetTable king_target_table;

extern const TargetTable north_ray_target_table;
extern const TargetTable northeast_ray_target_table;
extern const TargetTable east_ray_target_table;
extern const TargetTable southeast_ray_target_table;
extern const TargetTable south_ray_target_table;
extern const TargetTable southwest_ray_target_table;
extern const TargetTable west_ray_target_table;
extern const TargetTable northwest_ray_target_table;

auto get_target_table(const PieceType &piece_type) -> const TargetTable &;

using RayTargetTable = Table<TargetTable, ray_direction_count, RayDirection>;
extern const RayTargetTable ray_target_table;

using RankTable = Table<Bitmap, 8, Rank>;
using FileTable = Table<Bitmap, 8, File>;

extern const RankTable rank_table;
extern const FileTable file_table;

} // namespace chesscore::bitmaps

#endif
