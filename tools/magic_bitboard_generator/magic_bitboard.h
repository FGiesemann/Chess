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
#include <utility>
#include <vector>

/**
 * \brief A magic table specification.
 *
 * A table of magic bitboards is specified by the type of the sliding piece
 * (either rook or bishop) and the square on which the piece is placed.
 */
struct TableSpec {
    chesscore::PieceType piece{}; //< The type of the sliding piece.
    chesscore::Square square;     //< The square on which the piece is placed.
};

/**
 * \brief Parameters of a magic bitboard.
 *
 * The magic bitboard is specified by its magic number and shift.
 */
struct Magics {
    std::uint64_t magic_number; //< Magic number.
    std::uint64_t shift;        //< Shift.
};

/**
 * \brief The result of a magic bitboard search.
 *
 * The search result is a table of magic bitboards.
 */
using Table = std::vector<chesscore::Bitmap>;

/**
 * \brief Results and statistics from a magic bitboard generator run.
 *
 * This stores the result and statistics for a run of the table generator for
 * given magic values.
 */
struct GeneratorResult {
    std::uint64_t expected_entries{};             //< The number of blocker configurations.
    std::uint64_t stored_entries{};               //< Number of entries that could be stored in the table.
    std::uint64_t constructive_collisions{};      //< The number of constructive collisions during the generation.
    std::uint64_t max_index{};                    //< The maximum index in the table.
    std::optional<std::uint64_t> collision_index; //< The index at which a collision occurred.
    Table table;                                  //< The generated table.

    /**
     * \brief Check if the search was successful.
     *
     * A successfull search has no (destructive) collision.
     * \return If the search was successful.
     */
    [[nodiscard]] auto successful() const -> bool { return !collision_index.has_value(); }
};

/**
 * \brief The result of a magic bitboard search.
 *
 * The search result is a set of magic values and the table of magic bitboards.
 */
struct SearchResult {
    GeneratorResult generator_result; //< Result from filling the table with the found magics.
    Magics magics{};                  //< The magic values.
    std::uint64_t tries{};            //< Number of iterations until the magics were found.
};

/**
 * \brief Callback function to report search progress.
 */
using ProcessReportCallback = std::function<void(const SearchResult &)>;

/**
 * \brief A list of shift values.
 */
using Shifts = std::vector<std::uint64_t>;

/**
 * \brief Make a continuous range of shifts.
 *
 * Creates a list of shifts between start and end (both inclusive). Can be
 * ascending and descending.
 * \param start First shift in the sequence.
 * \param end Last shift in the sequence.
 * \return The sequence of shift values.
 */
auto make_shift_range(std::uint64_t start, std::uint64_t end) -> Shifts;

/**
 * \brief Parameters for the search.
 */
struct SearchParams {
    int max_tries{}; //< Maximum number of candidate magic numbers.
    Shifts shifts;   //< The shifts to check.
};

/**
 * \brief Generator for magic bitboards.
 *
 * Searches for magic bitboards for a given sliding piece and square.
 */
class MagicBitboardGenerator {
public:
    /**
     * \brief Construct a new Magic Bitboard Generator object
     *
     * \param spec The piece type and square for the table.
     */
    MagicBitboardGenerator(const TableSpec &spec) : m_spec{spec} {}

    /**
     * \brief Search for magics.
     *
     * Searches for magic values given the search parameters.
     * \param params Search parameters.
     * \return Result of the search, including magics and statistics.
     */
    auto search(const SearchParams &params) -> SearchResult;

    /**
     * \brief Try to fill a table with a given magic number and shift.
     *
     * \param magics The magic numbers.
     */
    [[nodiscard]] auto fill_table(const Magics &magics) const -> GeneratorResult;

    /**
     * \brief Set the early exit flag.
     *
     * \param early_exit Whether to early exit.
     */
    auto early_exit(bool early_exit) -> void { m_early_exit = early_exit; }

    /**
     * \brief Set the progress callback object
     *
     * \param callback The callback function called on search progress.
     */
    auto set_progress_callback(ProcessReportCallback callback) -> void { m_progress_callback = std::move(callback); }

    /**
     * \brief Set the report all magics flag
     *
     * \param report Whether to report all magics, or only those leading to smaller tables.
     */
    auto report_all_magics(bool report) -> void { m_report_all_magics = report; }

    /**
     * \brief Set the random seed value
     *
     * \param seed The seed.
     */
    auto set_rand_seed(std::uint64_t seed) -> void { m_rand_seed = seed; }
private:
    TableSpec m_spec;
    bool m_early_exit{false};
    ProcessReportCallback m_progress_callback;
    bool m_report_all_magics{false};
    std::uint64_t m_rand_seed{};

    auto update_result(SearchResult &search_result, const Magics &magics, const GeneratorResult &result) -> bool;
};

#endif
