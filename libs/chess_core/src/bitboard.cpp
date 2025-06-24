/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/bitboard.h"

namespace chesscore {

auto Bitboard::empty() const -> bool {
    return m_all_pieces.empty();
}

auto Bitboard::has_piece(const PieceType &piece_type) const -> bool {
    return !(bitmap({piece_type, Color::White}) | bitmap({piece_type, Color::Black})).empty();
}

auto Bitboard::has_piece(const Piece &piece) const -> bool {
    return !bitmap(piece).empty();
}

auto Bitboard::has_piece(const Color &color) const -> bool {
    return !bitmap(color).empty();
}

auto Bitboard::set_piece(const Piece &piece, const Square &square) -> void {
    bitmap(piece).set(square);
    m_all_pieces.set(square);
    bitmap(piece.color).set(square);
}

auto Bitboard::get_piece([[maybe_unused]] const Square &square) const -> std::optional<Piece> {
    return {};
}

auto Bitboard::clear_square([[maybe_unused]] const Square &square) -> void {}

auto Bitboard::set_from_fen([[maybe_unused]] const FenString &fen) -> void {}

auto Bitboard::calculate_hash_component() const -> uint64_t {
    return {};
}

} // namespace chesscore
