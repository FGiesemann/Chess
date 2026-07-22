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

    [[nodiscard]] auto successful() const -> bool { return !collision_index.has_value(); }
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
    int max_tries{};
    Shifts shifts;
};

class MagicBitboardGenerator {
public:
    MagicBitboardGenerator(const TableSpec &spec) : m_spec{spec} {}

    auto search(const SearchParams &params) -> SearchResult;

    /**
     * \brief Try to fill a table with a given magic number and shift.
     *
     * \param magics The magic numbers.
     */
    [[nodiscard]] auto fill_table(const Magics &magics) const -> GeneratorResult;

    auto early_exit(bool early_exit) -> void { m_early_exit = early_exit; }
    auto set_progress_callback(ProcessReportCallback callback) -> void { m_progress_callback = std::move(callback); }
    auto report_all_magics(bool report) -> void { m_report_all_magics = report; }
    auto set_rand_seed(std::uint64_t seed) -> void { m_rand_seed = seed; }
private:
    TableSpec m_spec;
    bool m_early_exit{false};
    ProcessReportCallback m_progress_callback;
    bool m_report_all_magics{false};
    std::uint64_t m_rand_seed{};

    auto update_result(SearchResult &search_result, const Magics &magics, const GeneratorResult &result) -> bool;
};

auto make_shift_range(std::uint64_t start, std::uint64_t end) -> Shifts;

#endif
