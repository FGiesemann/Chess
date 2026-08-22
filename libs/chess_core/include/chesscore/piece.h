/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_PIECE_H
#define CHESSCORE_PIECE_H

#include "chesscore/square.h"
#include "chesscore/table.h"

#include <array>
#include <cstdint>
#include <string>

namespace chesscore {

/**
 * \brief Type of a piece.
 */
enum class PieceType : std::int8_t {
    Pawn = 0,
    Knight = 1,
    Bishop = 2,
    Rook = 3,
    Queen = 4,
    King = 5,
    None = 6,
};

/**
 * \brief Number of available piece types.
 */
constexpr std::size_t piece_type_count = 6ULL;

/**
 * \brief All the piece types.
 *
 */
constexpr std::array<PieceType, piece_type_count> all_piece_types{PieceType::Pawn, PieceType::Knight, PieceType::Bishop, PieceType::Rook, PieceType::Queen, PieceType::King};

auto to_string(PieceType type) -> std::string;

/**
 * \brief Get the numeric index of a piece type.
 *
 * The piece types are enumerated pawn = 0, knight = 1, bishop = 2, rook = 3,
 * queen = 4, king = 5.
 * \param type The piece type.
 * \return The numerix index of the piece type.
 */
constexpr auto get_index(PieceType type) -> std::size_t {
    return static_cast<std::size_t>(type);
}

/**
 * \brief All the piece types that a pawn can promote into.
 */
constexpr std::array<PieceType, 4> all_promotion_piece_types{PieceType::Rook, PieceType::Knight, PieceType::Bishop, PieceType::Queen};

/**
 * \brief Give a piece type from an index.
 *
 * The piece types are ordered as Pawn, Rook, Knight, Bishop, Queen, King.
 * \param index The index.
 * \return Piece type corresponding to the index.
 */
constexpr auto piece_type_from_index(std::size_t index) noexcept -> PieceType {
    return static_cast<PieceType>(index);
}

/**
 * \brief Converts a character to a piece type.
 *
 * Converts a character as it may appear in a FEN string (r, n, b, q, k, p) to
 * the corresponding piece type. The function handles lowercase and uppercase
 * letters.
 * \param letter The letter to be converted.
 * \return The piece type correspinding to the letter.
 * \throws ChessException If the letter does not signify a valid piece type.
 */
auto piece_type_from_char(char letter) noexcept -> PieceType;

/**
 * \brief Color of a piece or player.
 */
enum class Color : std::int8_t {
    White = 0,
    Black = 1,
    None = 2,
};

auto to_string(Color color) noexcept -> std::string;

/**
 * \brief Swap a color.
 *
 * Switches white to black and black to white.
 * \param color The color to switch.
 * \return The switched color.
 */
constexpr auto other_color(Color color) noexcept -> Color {
    return color == Color::White ? Color::Black : Color::White;
}

/**
 * \brief A chess piece.
 *
 * A piece has a type and a color.
 */
class Piece {
private:
    /**
     * \brief Single-byte values for pieces.
     */
    enum class Value : std::uint8_t {
        WhitePawn = 0,   ///< Value for a white pawn.
        WhiteKnight = 1, ///< Value for a white knight.
        WhiteBishop = 2, ///< Value for a white bishop.
        WhiteRook = 3,   ///< Value for a white rook.
        WhiteQueen = 4,  ///< Value for a white queen.
        WhiteKing = 5,   ///< Value for a white king.

        BlackPawn = 8,    ///< Value for a black pawn.
        BlackKnight = 9,  ///< Value for a black knight.
        BlackBishop = 10, ///< Value for a black bishop.
        BlackRook = 11,   ///< Value for a black rook.
        BlackQueen = 12,  ///< Value for a black queen.
        BlackKing = 13,   ///< Value for a black king.

        None = 14, ///< Sentinal value for no piece.
    };
    std::uint8_t m_piece{static_cast<std::int8_t>(Value::None)}; ///< The value of the piece.

    static constexpr std::uint8_t type_mask = 0x07; ///< Mask to extract the piece type from the value.
    static constexpr std::uint8_t color_shift = 3;  ///< Shift to extract the color from the value.

    /**
     * \brief Construct a piece with a value.
     *
     * \param value The value.
     */
    constexpr Piece(Value value) noexcept : m_piece{static_cast<std::uint8_t>(value)} {}
public:
    /**
     * \brief Construct an invalid (None) piece.
     */
    constexpr Piece() noexcept = default;

    /**
     * \brief Construct a Piece object with the given type and color.
     *
     * \param type The type of the piece.
     * \param color The color of the piece.
     */
    Piece(PieceType type, Color color) noexcept : m_piece{static_cast<std::uint8_t>(static_cast<std::uint8_t>(type) | (static_cast<std::uint8_t>(color) << color_shift))} {}

    /** \brief Get the type of the piece. */
    [[nodiscard]] constexpr auto type() const noexcept -> PieceType { return static_cast<PieceType>(m_piece & type_mask); }
    /** \brief Get the color of the piece. */
    [[nodiscard]] constexpr auto color() const noexcept -> Color { return static_cast<Color>(m_piece >> color_shift); }
    /** \brief Get the value of the piece. */
    [[nodiscard]] constexpr auto value() const noexcept -> std::uint8_t { return static_cast<std::uint8_t>(m_piece); }
    /**
     * \brief Compute an index for the piece.
     *
     * The dense index can be used to index into tables.
     * \return An index of the piece.
     */
    [[nodiscard]] constexpr auto dense_index() const noexcept -> std::uint8_t { return static_cast<std::uint8_t>(m_piece - (color() == Color::Black ? 2 : 0)); }

    /** \brief Get the character representation of the piece. */
    [[nodiscard]] constexpr auto piece_char() const noexcept -> char { return "PNBRQKpnbrqk."[dense_index()]; }
    /** \brief Get the character representation of the piece ignoring color. */
    [[nodiscard]] constexpr auto piece_char_colorless() const noexcept -> char { return "PNBRQK."[get_index(type())]; }

    [[nodiscard]] constexpr auto is_piece() const noexcept -> bool { return m_piece != static_cast<std::uint8_t>(Value::None); }

    constexpr auto operator==(const Piece &) const noexcept -> bool = default;

    static const Piece None;
    static const Piece WhitePawn;
    static const Piece WhiteKnight;
    static const Piece WhiteBishop;
    static const Piece WhiteRook;
    static const Piece WhiteQueen;
    static const Piece WhiteKing;
    static const Piece BlackPawn;
    static const Piece BlackKnight;
    static const Piece BlackBishop;
    static const Piece BlackRook;
    static const Piece BlackQueen;
    static const Piece BlackKing;
};

static_assert(sizeof(Piece) == 1, "Piece must be 1 byte");

inline constexpr Piece Piece::None{Piece::Value::None};
inline constexpr Piece Piece::WhitePawn{Piece::Value::WhitePawn};
inline constexpr Piece Piece::WhiteKnight{Piece::Value::WhiteKnight};
inline constexpr Piece Piece::WhiteBishop{Piece::Value::WhiteBishop};
inline constexpr Piece Piece::WhiteRook{Piece::Value::WhiteRook};
inline constexpr Piece Piece::WhiteQueen{Piece::Value::WhiteQueen};
inline constexpr Piece Piece::WhiteKing{Piece::Value::WhiteKing};
inline constexpr Piece Piece::BlackPawn{Piece::Value::BlackPawn};
inline constexpr Piece Piece::BlackKnight{Piece::Value::BlackKnight};
inline constexpr Piece Piece::BlackBishop{Piece::Value::BlackBishop};
inline constexpr Piece Piece::BlackRook{Piece::Value::BlackRook};
inline constexpr Piece Piece::BlackQueen{Piece::Value::BlackQueen};
inline constexpr Piece Piece::BlackKing{Piece::Value::BlackKing};

/**
 * \brief Convert a piece to string.
 *
 * \param piece The piece.
 * \return A string representation of the piece.
 */
auto to_string(Piece piece) noexcept -> std::string;

/**
 * \brief Converts a character to a piece.
 *
 * Converts a letter as it may appear in a FEN string (r, n, b, q, k, p) to a
 * chess piece. Lowercase letters stand for black pieces, uppercase letters
 * stand for white pieces.
 * \param letter The letter to be converted.
 * \return Piece Corresponding piece.
 * \throws ChessException If the letter does not signify a valid piece.
 */
auto piece_from_fen_letter(char letter) noexcept -> Piece;

/**
 * \brief A description of pieces on a chess board.
 *
 * This is a simple representation of the placement of pieces on a chess board.
 * The 64 squares are put into the list one rank after the other, starting with
 * rank 0.
 */
using PiecePlacement = std::array<Piece, Square::count>;

/**
 * \brief Generate a piece placement from a string.
 *
 * The string has to have length 64 and each character stand for a piece in one
 * of the squares of the chess board. It begins with position a1, then a2..a8,
 * b1..b8, ..., h1..h8.
 *
 * Each Character is either an underscore (_) denoting an empty square, or a
 * character p, r, n, b, q, k. Lowercase letters stand for black pieces,
 * uppercase letters stand for white pieces.
 *
 * \param str The string to interpret.
 * \return A corresponding piece placement.
 */
auto placement_from_string(const std::string &str) -> PiecePlacement;

/**
 * \brief Piece placement for the opening configuration of a regular chess game.
 *
 * Returns the piece placement for the opening configuration of a regular chess
 * game.
 * \return Piece placement for regular chess game.
 */
auto starting_piece_placement() -> PiecePlacement;

/**
 * \brief Type for a table of directions for each piece.
 *
 * The directions are stored in a bitfield.
 */
using PieceDirections = Table<std::uint8_t, piece_type_count, PieceType>;

/**
 * \brief A table storing ray directions for pieces.
 *
 * Stores the directions in which a sliding piee can move.
 */
extern const PieceDirections piece_ray_directions;

} // namespace chesscore

#endif
