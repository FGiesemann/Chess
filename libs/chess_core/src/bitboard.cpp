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
    return !(bitmap({.type = piece_type, .color = Color::White}) | bitmap({.type = piece_type, .color = Color::Black})).empty();
}

auto Bitboard::has_piece(const Piece &piece) const -> bool {
    return !bitmap(piece).empty();
}

auto Bitboard::has_piece(const Color &color) const -> bool {
    return !bitmap(color).empty();
}

auto Bitboard::has_piece(const Square &square) const -> bool {
    return m_all_pieces.get(square);
}

auto Bitboard::set_piece(const Piece &piece, const Square &square) -> void {
    bitmap(piece).set(square);
    m_all_pieces.set(square);
    bitmap(piece.color).set(square);
}

auto Bitboard::get_piece(const Square &square) const -> std::optional<Piece> {
    if (!m_all_pieces.get(square)) {
        return {};
    }
    const auto [min_index, max_index] = (m_white_pieces.get(square)) ? std::make_pair(min_white_piece_index, max_white_piece_index) : std::make_pair(min_black_piece_index, max_black_piece_index);
    for (auto i = min_index; i <= max_index; ++i) {
        if (m_bitmaps.at(i).get(square)) {
            return Piece{.type = static_cast<PieceType>(i - min_index), .color = (i <= max_white_piece_index) ? Color::White : Color::Black};
        }
    }
    return {};
}

auto Bitboard::clear_square([[maybe_unused]] const Square &square) -> void {}

auto Bitboard::set_from_fen([[maybe_unused]] const FenString &fen) -> void {}

auto Bitboard::calculate_hash_component() const -> uint64_t {
    return {};
}

} // namespace chesscore
