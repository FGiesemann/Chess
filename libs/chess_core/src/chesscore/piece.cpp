/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/piece.h"
#include "chesscore/chesscore.h"

#include <cctype>

namespace chesscore {

auto to_string(PieceType type) -> std::string {
    switch (type) {
    case PieceType::None:
        return "X";
    case PieceType::Pawn:
        return "P";
    case PieceType::Rook:
        return "R";
    case PieceType::Knight:
        return "K";
    case PieceType::Bishop:
        return "B";
    case PieceType::Queen:
        return "Q";
    case PieceType::King:
        return "K";
    }
    throw ChessException("Invalid piece type");
}

auto piece_type_from_char(char letter) noexcept -> PieceType {
    switch (std::tolower(letter)) {
    case 'r':
        return PieceType::Rook;
    case 'n':
        return PieceType::Knight;
    case 'b':
        return PieceType::Bishop;
    case 'q':
        return PieceType::Queen;
    case 'k':
        return PieceType::King;
    case 'p':
        return PieceType::Pawn;
    default:
        return PieceType::None;
    }
}

auto to_string(Piece piece) noexcept -> std::string {
    return std::string{piece.piece_char()};
}

auto piece_from_fen_letter(char letter) noexcept -> Piece {
    const auto color = std::isupper(letter) != 0 ? Color::White : Color::Black;
    return Piece{piece_type_from_char(letter), color};
}

auto to_string(Color color) noexcept -> std::string {
    return color == Color::White ? "White" : "Black";
}

auto placement_from_string(const std::string &str) -> PiecePlacement {
    PiecePlacement placement{};
    static constexpr size_t placement_length{64};
    if (str.length() != placement_length) {
        throw ChessException("Invalid piece placement string: " + str);
    }
    for (size_t i = 0; i < placement_length; ++i) {
        const char letter = str[i];
        if (letter != '_') {
            placement.at(i) = piece_from_fen_letter(letter);
        }
    }
    return placement;
}

auto starting_piece_placement() -> PiecePlacement {
    return placement_from_string("RNBQKBNRPPPPPPPP________________________________pppppppprnbqkbnr");
}

const PieceDirections piece_ray_directions{
    std::uint8_t{0b0000'0000}, // Pawn
    std::uint8_t{0b0000'0000}, // Knight
    std::uint8_t{0b1010'1010}, // Bishop
    std::uint8_t{0b0101'0101}, // Rook
    std::uint8_t{0b1111'1111}, // Queen
    std::uint8_t{0b0000'0000}, // King
};

} // namespace chesscore
