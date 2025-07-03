/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include <bitset>
#include <iosfwd>

#include "chesscore/bitmap.h"

namespace chesscore::table_gen {

static constexpr std::size_t board_width{8};
static constexpr std::size_t overhang{board_width - 1};
static constexpr std::size_t bigmap_width{2 * overhang + board_width};
static constexpr std::size_t pattern_width{overhang + board_width};

using Pattern = std::bitset<pattern_width * pattern_width>;

extern const Pattern knight_pattern;

enum class ShiftDirection { North, South, East, West };

class Bigmap {
public:
    Bigmap() = default;
    Bigmap(const Pattern &pattern);

    using Bitset = std::bitset<bigmap_width * bigmap_width>;

    auto operator[](std::size_t pos) const -> bool { return m_bits[pos]; }
    auto at(std::size_t row, std::size_t col) const -> bool { return m_bits[index(row, col)]; }
    auto operator[](std::size_t pos) -> Bitset::reference { return m_bits[pos]; }

    auto shift(ShiftDirection dir) -> void;

    auto extract_board() const -> Bitmap;
    auto index(std::size_t row, std::size_t col) const -> std::size_t { return row * bigmap_width + col; }
    static auto inside_board(std::size_t row, std::size_t col) -> bool;
private:
    Bitset m_bits{};
};

auto operator<<(std::ostream &os, const Bigmap &bigmap) -> std::ostream &;

} // namespace chesscore::table_gen
