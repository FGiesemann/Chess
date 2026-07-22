/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#ifndef MAGIC_BITBOARD_H
#define MAGIC_BITBOARD_H

#include <chesscore/bitmap.h>
#include <chesscore/piece.h>
#include <chesscore/square.h>

#include <optional>
#include <vector>

struct TableSpec {
    chesscore::PieceType piece{};
    chesscore::Square square;
};

struct Magics {
    std::uint64_t magic_number;
    std::uint64_t shift;
};

using Table = std::vector<chesscore::Bitmap>;

struct GeneratorResult {
    std::uint64_t expected_entries{};
    std::uint64_t stored_entries{};
    std::uint64_t constructive_collisions{};
    std::uint64_t max_index{};
    std::optional<std::uint64_t> collision_index;
    Table table;
};

struct SearchParams {
    std::uint64_t rand_seed{};
    int max_tries{};
    std::uint64_t shift{};
    bool early_exit{false};
};

struct SearchResult {
    GeneratorResult generator_result;
    std::uint64_t magic_number{};
    std::uint64_t tries{};
};

/**
 * \brief Try to fill a table with a given magic number and shift.
 *
 * \param spec Specificaation of piece type and square.
 * \param magics The magic numbers.
 */
auto fill_table(const TableSpec &spec, const Magics &magics) -> GeneratorResult;

/**
 * \brief Try to find a magic number.
 *
 * Tries different randomly selected magic numbers until either a table can be
 * generated or the maximum number of tries is reached.
 * \param spec Specification of piece type and square.
 * \param params Maximum number of tries and shift.
 * \return The search result.
 */
auto search_magic_number(const TableSpec &spec, const SearchParams &params) -> SearchResult;

#endif
