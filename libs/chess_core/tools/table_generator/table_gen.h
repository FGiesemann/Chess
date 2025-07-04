/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#ifndef CHESSCORE_TABLE_GEN_TABLE_GEN_H
#define CHESSCORE_TABLE_GEN_TABLE_GEN_H

#include <iosfwd>
#include <string>

#include "bigmap.h"

namespace chesscore::table_gen {

auto generate_table(const std::string &name, const Pattern &pattern, std::ostream &os) -> void;

}

#endif
