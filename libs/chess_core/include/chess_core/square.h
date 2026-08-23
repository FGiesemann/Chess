/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_SQUARE_H
#define CHESSCORE_SQUARE_H

#include <cstdint>
#include <string>

namespace chesscore {

namespace detail {

[[nodiscard]] constexpr auto charToLower(const char character) noexcept -> char {
    return (character >= 'A' && character <= 'Z') ? static_cast<char>(character + ('a' - 'A')) : character;
}

} // namespace detail

/**
 * \brief A file (column) on the board.
 *
 * A file is a column on the board. It can be specified by a number 0..7 or by a
 * character a..h.
 */
struct File {
    static constexpr int count = 8; ///< The number of files.

    /**
     * \brief A file from its name.
     *
     * The name is a character in the range a..h. The name is case insensitive,
     * so 'A'..'H' are also valid.
     * \param in_file A character in the range a..h (case insensitive).
     */
    constexpr File(char in_file) noexcept : file{static_cast<int>(detail::charToLower(in_file) - 'a')} {}

    /**
     * \brief A file from its number.
     *
     * The file is specified as a number in the range 0..7.
     * \param in_file A number in the range 0..7.
     */
    constexpr File(int in_file) noexcept : file{in_file} {}

    int file; ///< The file number (0..7).

    /**
     * \brief Returns the file as a character.
     *
     * The name of the file is a character in the range a..h.
     * \return The name of the file.
     */
    [[nodiscard]] constexpr auto name() const noexcept -> char { return static_cast<char>(file + 'a'); }

    /**
     * \brief Step to the right.
     *
     * Shifts the file to the right by the given amount. Increment uses
     * wrap-around: If the file is moved over the edge, it "re-enters" from the
     * other side.
     * \param steps Number of fields to step to the right.
     * \return The modified file.
     */
    constexpr auto operator+=(int steps) noexcept -> File & {
        file = (file + steps) % count;
        return *this;
    }

    /**
     * \brief Equality comparison for files.
     *
     * Compare two file numbers for equality. They are equal, if they represent
     * the same file.
     * @param lhs Left-hand side of the comparison.
     * @param rhs Right-hand side of the comparison.
     * @return Equality of the two files.
     */
    [[nodiscard]] friend auto operator==(File lhs, File rhs) noexcept -> bool { return lhs.file == rhs.file; }
};

[[nodiscard]] constexpr auto operator<(File lhs, File rhs) noexcept -> bool {
    return lhs.file < rhs.file;
}

[[nodiscard]] constexpr auto get_index(File file) noexcept -> int {
    return file.file;
}

/**
 * \brief A rank (row) on the board.
 *
 * A rank is a row on the board. It can be specified by a number 0..7.
 */
struct Rank {
    static constexpr int count = 8; ///< The number of ranks.

    static constexpr int white_pawn_double_step_rank = 1; ///< The rank number from where white pawns can double step.
    static constexpr int black_pawn_double_step_rank = 6; ///< The rank number from where black pawns can double step.

    /**
     * \brief A rank from its number.
     *
     * The rank is a number in the range 0..7.
     * \param in_rank A number in the range 0..7.
     */
    constexpr Rank(int in_rank) noexcept : rank{in_rank} {}

    int rank; ///< The rank number (0..7).

    /**
     * \brief Step up.
     *
     * Shifts the rank up by the given amount. Increment uses wrap-around: If
     * the rank is moved over the edge, it "re-enters" from the other side.
     * \param steps Number of fields to step up.
     * \return The modified rank.
     */
    constexpr auto operator+=(int steps) noexcept -> Rank & {
        rank = (rank + steps) % count;
        return *this;
    }

    /**
     * \brief Equality comparison for ranks.
     *
     * Compare two rank numbers for equality. They are equal, if they represent
     * the same rank.
     * @param lhs Left-hand side of the comparison.
     * @param rhs Right-hand side of the comparison.
     * @return Equality of the two ranks.
     */
    [[nodiscard]] friend auto operator==(Rank lhs, Rank rhs) noexcept -> bool { return lhs.rank == rhs.rank; }
};

[[nodiscard]] constexpr auto operator<(Rank lhs, Rank rhs) noexcept -> bool {
    return lhs.rank < rhs.rank;
}

[[nodiscard]] constexpr auto get_index(Rank rank) noexcept -> int {
    return rank.rank;
}

/**
 * \brief A position on the board.
 *
 * Each square is identified by its file (the column) and the rank (the row).
 */
class Square {
public:
    /**
     * \brief Construct a new Square object with a given coordinates.
     *
     * A square is defined by its file (column) and rank (row).
     * \param file The file (column) of the square.
     * \param rank The rank (row) of the square.
     */
    constexpr Square(File file, Rank rank) noexcept : m_index{static_cast<std::uint8_t>((rank.rank << 3) | file.file)} {}

    /**
     * \brief Default construtor.
     *
     * Creates an invalid square.
     */
    constexpr Square() noexcept : Square(0, Rank::count) {}

    /**
     * \brief Mask to extract the file from the square index.
     */
    static constexpr std::uint8_t file_mask = 0x07;

    /**
     * \brief Access the file of the square.
     *
     * The file (column) of the square.
     * \return The file.
     */
    [[nodiscard]] constexpr auto file() const noexcept -> File { return File{m_index & file_mask}; }

    /**
     * \brief Access the rank of the square.
     *
     * The rank (row) of the square.
     * \return The rank.
     */
    [[nodiscard]] constexpr auto rank() const noexcept -> Rank { return Rank{m_index >> 3}; }

    /**
     * \brief Gives a linear index for the square.
     *
     * The suqares are indexed from 0 to 63, starting with A1 = 0, B1 = 1, ...,
     * H8 = 63.
     * \return Linear index of the square.
     */
    [[nodiscard]] constexpr auto index() const noexcept -> std::uint8_t { return m_index; }

    /**
     * \brief The number of squares on the board.
     */
    static constexpr int count = File::count * Rank::count;

    /**
     * \brief Mirrors the rank at the center line.
     *
     * Mirrors the rank of the square at the center line (between ranks 4 and
     * 5). This allows to "switch the player/color".
     * \return The mirrored square.
     */
    [[nodiscard]] auto mirrored() const noexcept -> Square { return Square{file(), Rank{Rank::count - rank().rank - 1}}; }

    /**
     * \brief Skip to the "next" square.
     *
     * Step from the current square to a following square. The squares are
     * enumerated according to their linear index, i.e., A1, B1, ..., H8.
     * No range checking is performed, steps can generate invalid squares!
     * \param squares The number of squares to skip.
     * \return The new Square.
     */
    constexpr auto operator+=(int squares) noexcept -> Square & {
        m_index = static_cast<std::uint8_t>(m_index + squares);
        return *this;
    }

    /**
     * \brief Skip to the "next" square.
     *
     * Step this square from the current square to a following square. No range
     * checking is performed, steps can generate invalid squares!
     * \return Reference to this updated square.
     */
    constexpr auto operator++() noexcept -> Square & { return *this += 1; }

    constexpr auto operator++(int) noexcept -> Square {
        Square old{*this};
        operator++();
        return old;
    }

    /**
     * \brief Skip back to a "previous" square.
     *
     * Step from the current square to a previous square. The squares are
     * enumerated according to their linear index, i.e., A1, B1, ..., H8.
     * No range checking is performed, steps can generate invalid squares!
     * \param squares The number of squares to skip.
     * \return The new Square.
     */
    constexpr auto operator-=(int squares) noexcept -> Square & {
        m_index = static_cast<std::uint8_t>(static_cast<int>(m_index) - squares);
        return *this;
    }

    /**
     * \brief Skip back to the "previous" square.
     *
     * Step from the current square to a previous square. The squares are
     * enumerated according to their linear index, i.e., A1, B1, ..., H8.
     * No range checking is performed, steps can generate invalid squares!
     * \return The new Square.
     */
    constexpr auto operator--() noexcept -> Square & { return *this -= 1; }

    /**
     * \brief Skip back to the "previous" square.
     *
     * Step from the current square to a previous square. The squares are
     * enumerated according to their linear index, i.e., A1, B1, ..., H8.
     * No range checking is performed, steps can generate invalid squares!
     * \return The new Square.
     */
    constexpr auto operator--(int) noexcept -> Square {
        Square old{*this};
        operator--();
        return old;
    }

    /**
     * \brief If the square is valid.
     *
     * Valid squares are in the range A1 to H8.
     * \return If the square is valid.
     */
    [[nodiscard]] constexpr auto valid() const noexcept -> bool { return m_index < count; }

    /**
     * \brief Equality comparison for square positions.
     *
     * Compare two square positions for equality. They are equal, if their files
     * and ranks are equal.
     * @param lhs Left-hand side of the comparison.
     * @param rhs Right-hand side of the comparison.
     * @return Equality of the two square positions.
     */
    [[nodiscard]] friend auto operator==(Square lhs, Square rhs) noexcept -> bool { return lhs.file() == rhs.file() && lhs.rank() == rhs.rank(); }

    ///@{
    /**
     * \name Squares on a chess board
     * \brief Pre-defined square positions on a chess board.
     *
     * The square names correspond with the default naming of chess boards.
     */
    // NOLINTBEGIN(readability-identifier-length)
    static const Square A1; ///< The square A1.
    static const Square A2; ///< The square A2.
    static const Square A3; ///< The square A3.
    static const Square A4; ///< The square A4.
    static const Square A5; ///< The square A5.
    static const Square A6; ///< The square A6.
    static const Square A7; ///< The square A7.
    static const Square A8; ///< The square A8.
    static const Square B1; ///< The square B1.
    static const Square B2; ///< The square B2.
    static const Square B3; ///< The square B3.
    static const Square B4; ///< The square B4.
    static const Square B5; ///< The square B5.
    static const Square B6; ///< The square B6.
    static const Square B7; ///< The square B7.
    static const Square B8; ///< The square B8.
    static const Square C1; ///< The square C1.
    static const Square C2; ///< The square C2.
    static const Square C3; ///< The square C3.
    static const Square C4; ///< The square C4.
    static const Square C5; ///< The square C5.
    static const Square C6; ///< The square C6.
    static const Square C7; ///< The square C7.
    static const Square C8; ///< The square C8.
    static const Square D1; ///< The square D1.
    static const Square D2; ///< The square D2.
    static const Square D3; ///< The square D3.
    static const Square D4; ///< The square D4.
    static const Square D5; ///< The square D5.
    static const Square D6; ///< The square D6.
    static const Square D7; ///< The square D7.
    static const Square D8; ///< The square D8.
    static const Square E1; ///< The square E1.
    static const Square E2; ///< The square E2.
    static const Square E3; ///< The square E3.
    static const Square E4; ///< The square E4.
    static const Square E5; ///< The square E5.
    static const Square E6; ///< The square E6.
    static const Square E7; ///< The square E7.
    static const Square E8; ///< The square E8.
    static const Square F1; ///< The square F1.
    static const Square F2; ///< The square F2.
    static const Square F3; ///< The square F3.
    static const Square F4; ///< The square F4.
    static const Square F5; ///< The square F5.
    static const Square F6; ///< The square F6.
    static const Square F7; ///< The square F7.
    static const Square F8; ///< The square F8.
    static const Square G1; ///< The square G1.
    static const Square G2; ///< The square G2.
    static const Square G3; ///< The square G3.
    static const Square G4; ///< The square G4.
    static const Square G5; ///< The square G5.
    static const Square G6; ///< The square G6.
    static const Square G7; ///< The square G7.
    static const Square G8; ///< The square G8.
    static const Square H1; ///< The square H1.
    static const Square H2; ///< The square H2.
    static const Square H3; ///< The square H3.
    static const Square H4; ///< The square H4.
    static const Square H5; ///< The square H5.
    static const Square H6; ///< The square H6.
    static const Square H7; ///< The square H7.
    static const Square H8; ///< The square H8.

    static const Square None; ///< An invalid square.
    // NOLINTEND(readability-identifier-length)
    ///@}
private:
    std::uint8_t m_index{count}; ///< The linear index of the square.
};

// NOLINTBEGIN(readability-identifier-length)
inline constexpr Square Square::A1{File{'a'}, Rank{0}};
inline constexpr Square Square::A2{File{'a'}, Rank{1}};
inline constexpr Square Square::A3{File{'a'}, Rank{2}};
inline constexpr Square Square::A4{File{'a'}, Rank{3}};
inline constexpr Square Square::A5{File{'a'}, Rank{4}};
inline constexpr Square Square::A6{File{'a'}, Rank{5}};
inline constexpr Square Square::A7{File{'a'}, Rank{6}};
inline constexpr Square Square::A8{File{'a'}, Rank{7}};

inline constexpr Square Square::B1{File{'b'}, Rank{0}};
inline constexpr Square Square::B2{File{'b'}, Rank{1}};
inline constexpr Square Square::B3{File{'b'}, Rank{2}};
inline constexpr Square Square::B4{File{'b'}, Rank{3}};
inline constexpr Square Square::B5{File{'b'}, Rank{4}};
inline constexpr Square Square::B6{File{'b'}, Rank{5}};
inline constexpr Square Square::B7{File{'b'}, Rank{6}};
inline constexpr Square Square::B8{File{'b'}, Rank{7}};

inline constexpr Square Square::C1{File{'c'}, Rank{0}};
inline constexpr Square Square::C2{File{'c'}, Rank{1}};
inline constexpr Square Square::C3{File{'c'}, Rank{2}};
inline constexpr Square Square::C4{File{'c'}, Rank{3}};
inline constexpr Square Square::C5{File{'c'}, Rank{4}};
inline constexpr Square Square::C6{File{'c'}, Rank{5}};
inline constexpr Square Square::C7{File{'c'}, Rank{6}};
inline constexpr Square Square::C8{File{'c'}, Rank{7}};

inline constexpr Square Square::D1{File{'d'}, Rank{0}};
inline constexpr Square Square::D2{File{'d'}, Rank{1}};
inline constexpr Square Square::D3{File{'d'}, Rank{2}};
inline constexpr Square Square::D4{File{'d'}, Rank{3}};
inline constexpr Square Square::D5{File{'d'}, Rank{4}};
inline constexpr Square Square::D6{File{'d'}, Rank{5}};
inline constexpr Square Square::D7{File{'d'}, Rank{6}};
inline constexpr Square Square::D8{File{'d'}, Rank{7}};

inline constexpr Square Square::E1{File{'e'}, Rank{0}};
inline constexpr Square Square::E2{File{'e'}, Rank{1}};
inline constexpr Square Square::E3{File{'e'}, Rank{2}};
inline constexpr Square Square::E4{File{'e'}, Rank{3}};
inline constexpr Square Square::E5{File{'e'}, Rank{4}};
inline constexpr Square Square::E6{File{'e'}, Rank{5}};
inline constexpr Square Square::E7{File{'e'}, Rank{6}};
inline constexpr Square Square::E8{File{'e'}, Rank{7}};

inline constexpr Square Square::F1{File{'f'}, Rank{0}};
inline constexpr Square Square::F2{File{'f'}, Rank{1}};
inline constexpr Square Square::F3{File{'f'}, Rank{2}};
inline constexpr Square Square::F4{File{'f'}, Rank{3}};
inline constexpr Square Square::F5{File{'f'}, Rank{4}};
inline constexpr Square Square::F6{File{'f'}, Rank{5}};
inline constexpr Square Square::F7{File{'f'}, Rank{6}};
inline constexpr Square Square::F8{File{'f'}, Rank{7}};

inline constexpr Square Square::G1{File{'g'}, Rank{0}};
inline constexpr Square Square::G2{File{'g'}, Rank{1}};
inline constexpr Square Square::G3{File{'g'}, Rank{2}};
inline constexpr Square Square::G4{File{'g'}, Rank{3}};
inline constexpr Square Square::G5{File{'g'}, Rank{4}};
inline constexpr Square Square::G6{File{'g'}, Rank{5}};
inline constexpr Square Square::G7{File{'g'}, Rank{6}};
inline constexpr Square Square::G8{File{'g'}, Rank{7}};

inline constexpr Square Square::H1{File{'h'}, Rank{0}};
inline constexpr Square Square::H2{File{'h'}, Rank{1}};
inline constexpr Square Square::H3{File{'h'}, Rank{2}};
inline constexpr Square Square::H4{File{'h'}, Rank{3}};
inline constexpr Square Square::H5{File{'h'}, Rank{4}};
inline constexpr Square Square::H6{File{'h'}, Rank{5}};
inline constexpr Square Square::H7{File{'h'}, Rank{6}};
inline constexpr Square Square::H8{File{'h'}, Rank{7}};

inline constexpr Square Square::None{File{'a'}, Rank{8}};
// NOLINTEND(readability-identifier-length)

static_assert(sizeof(Square) == 1, "Square must be 1 byte");

/**
 * \brief Skip to the "next" square.
 *
 * Step from the given square to a following square by the given amount.
 * \param square The square to start from.
 * \param squares The number of squares to skip.
 * \return The new Square.
 */
[[nodiscard]] constexpr auto operator+(Square square, int squares) noexcept -> Square {
    Square result{square};
    result += squares;
    return result;
}

/**
 * \brief Skip to the "previous" square.
 *
 * Step from the given square to a previous square by the given amount.
 * \param square The square to start from.
 * \param squares The number of squares to skip.
 * \return The new Square.
 */
[[nodiscard]] constexpr auto operator-(Square square, int squares) noexcept -> Square {
    Square result{square};
    result -= squares;
    return result;
}

[[nodiscard]] constexpr auto to_string(Square square) noexcept -> std::string {
    return std::string{square.file().name()} + std::to_string(square.rank().rank + 1);
}

} // namespace chesscore

#endif
