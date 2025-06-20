/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/piece.h"

#include <cctype>

namespace chesscore {

auto piece_type_from_char(char piece_type) -> PieceType {
    switch (std::tolower(piece_type)) {
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
        throw ChessException("Invalid piece type: " + std::string{piece_type});
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

} // namespace chesscore
