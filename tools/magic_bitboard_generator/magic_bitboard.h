/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#ifndef MAGIC_BITBOARD_H
#define MAGIC_BITBOARD_H

#include <chesscore/bitmap.h>
#include <chesscore/piece.h>
#include <chesscore/square.h>

#include <functional>
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

struct SearchResult {
    GeneratorResult generator_result;
    std::uint64_t magic_number{};
    std::uint64_t shift{};
    std::uint64_t tries{};
};

using ProcessReportCallback = std::function<void(const SearchResult &)>;
using Shifts = std::vector<std::uint64_t>;

struct SearchParams {
    std::uint64_t rand_seed{};
    int max_tries{};
    Shifts shifts;
    bool early_exit{false};
    std::optional<ProcessReportCallback> process_report_callback;
    bool report_all_magics{false};
};

auto make_shift_range(std::uint64_t start, std::uint64_t end) -> Shifts;

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
 * \param params Maximum number of tries and shifts.
 * \return The search result.
 */
auto search_magic_number(const TableSpec &spec, const SearchParams &params) -> SearchResult;

#endif
