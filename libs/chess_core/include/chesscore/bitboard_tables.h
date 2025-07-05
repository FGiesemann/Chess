/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_BITBOARD_TABLES_H
#define CHESSCORE_BITBOARD_TABLES_H

#include "chesscore/bitmap.h"
#include "chesscore/piece.h"
#include "chesscore/square.h"
#include "chesscore/table.h"

namespace chesscore {

using TargetTable = Table<Bitmap, 64, Square>;

extern const TargetTable knight_target_table;
extern const TargetTable bishop_target_table;
extern const TargetTable rook_target_table;
extern const TargetTable queen_target_table;
extern const TargetTable king_target_table;

using PieceTargetTables = Table<const TargetTable *, piece_type_count, PieceType>;

extern const PieceTargetTables piece_target_tables;

} // namespace chesscore

#endif
