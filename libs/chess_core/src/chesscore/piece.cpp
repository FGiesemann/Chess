/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/piece.h"

#include <cctype>

namespace chesscore {

auto piece_type_from_index(int index) -> PieceType {
    if (index >= 0 && index < piece_type_count) {
        return static_cast<PieceType>(index);
    }
    throw ChessException("Invalid piece type index: " + std::to_string(index));
}

auto piece_type_from_char(char letter) -> PieceType {
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
        throw ChessException("Invalid piece type: " + std::string{letter});
    }
}

auto piece_from_fen_letter(char letter) -> Piece {
    if (std::isupper(letter) != 0) {
        return Piece{.type = piece_type_from_char(letter), .color = Color::White};
    }
    return Piece{.type = piece_type_from_char(letter), .color = Color::Black};
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

const Piece Piece::WhitePawn{.type = PieceType::Pawn, .color = Color::White};
const Piece Piece::WhiteRook{.type = PieceType::Rook, .color = Color::White};
const Piece Piece::WhiteKnight{.type = PieceType::Knight, .color = Color::White};
const Piece Piece::WhiteBishop{.type = PieceType::Bishop, .color = Color::White};
const Piece Piece::WhiteQueen{.type = PieceType::Queen, .color = Color::White};
const Piece Piece::WhiteKing{.type = PieceType::King, .color = Color::White};

const Piece Piece::BlackPawn{.type = PieceType::Pawn, .color = Color::Black};
const Piece Piece::BlackRook{.type = PieceType::Rook, .color = Color::Black};
const Piece Piece::BlackKnight{.type = PieceType::Knight, .color = Color::Black};
const Piece Piece::BlackBishop{.type = PieceType::Bishop, .color = Color::Black};
const Piece Piece::BlackQueen{.type = PieceType::Queen, .color = Color::Black};
const Piece Piece::BlackKing{.type = PieceType::King, .color = Color::Black};

const PieceDirections piece_ray_directions{
    std::uint8_t{0b0000'0000}, // Pawn
    std::uint8_t{0b0101'0101}, // Rook
    std::uint8_t{0b0000'0000}, // Knight
    std::uint8_t{0b1010'1010}, // Bishop
    std::uint8_t{0b1111'1111}, // Queen
    std::uint8_t{0b0000'0000}, // King
};

} // namespace chesscore
