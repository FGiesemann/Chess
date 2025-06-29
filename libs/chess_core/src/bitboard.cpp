/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/bitboard.h"

namespace chesscore {

Bitboard::Bitboard(const FenString &fen) {
    const auto &placements{fen.piece_placement()};
    for (int rank{Rank::min_rank}; rank <= Rank::max_rank; ++rank) {
        for (int file{File::min_file}; file <= File::max_file; ++file) {
            const auto square = Square{rank, file};
            const auto piece{placements[square.index()]};
            if (piece) {
                set_piece(piece.value(), square);
            }
        }
    }
}

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
    clear_square(square);
    bitmap(piece).set(square);
    m_all_pieces.set(square);
    bitmap(piece.color).set(square);
}

auto Bitboard::get_piece(const Square &square) const -> std::optional<Piece> {
    if (!m_all_pieces.get(square)) {
        return {};
    }
    const auto [min_index, max_index] =
        (m_white_pieces.get(square)) ? std::make_pair(min_white_piece_index, max_white_piece_index) : std::make_pair(min_black_piece_index, max_black_piece_index);
    for (auto i = min_index; i <= max_index; ++i) {
        if (m_bitmaps.at(i).get(square)) {
            return Piece{.type = static_cast<PieceType>(i - min_index), .color = (i <= max_white_piece_index) ? Color::White : Color::Black};
        }
    }
    return {};
}

auto Bitboard::clear_square(const Square &square) -> void {
    const auto remove_from_square = ~Bitmap{square};

    for (auto &bitmap : m_bitmaps) {
        bitmap &= remove_from_square;
    }

    m_white_pieces &= remove_from_square;
    m_black_pieces &= remove_from_square;
    m_all_pieces &= remove_from_square;
}

auto Bitboard::make_move(const Move &move) -> void {
    clear_square(move.from);
    if (move.promoted) {
        set_piece(move.promoted.value(), move.to);
    } else {
        set_piece(move.piece, move.to);
    }
    if (move.is_castling()) {
        move_castling_rook(move);
    }
    if (move.capturing_en_passant) {
        clear_square(Square{move.to.file(), move.from.rank()});
    }
}

void Bitboard::move_castling_rook(const Move &move) {
    if (move.from.file().file < move.to.file().file) {
        // Kingside castling
        clear_square(Square{File{'H'}, move.to.rank()});                                                         // remove rook
        set_piece(Piece{.type = PieceType::Rook, .color = move.piece.color}, Square{File{'F'}, move.to.rank()}); // place rook on f-file
    } else {
        // Queenside castling
        clear_square(Square{File{'A'}, move.to.rank()});                                                         // remove rook
        set_piece(Piece{.type = PieceType::Rook, .color = move.piece.color}, Square{File{'D'}, move.to.rank()}); // place rook on d-file
    }
}

auto Bitboard::unmake_move(const Move &move) -> void {
    set_piece(move.piece, move.from);
    if (move.captured) {
        if (move.capturing_en_passant) {
            set_piece(move.captured.value(), Square{move.to.file(), move.from.rank()});
            clear_square(move.to);
        } else {
            set_piece(move.captured.value(), move.to);
        }
    } else {
        clear_square(move.to);
    }
    if (move.is_castling()) {
        reset_castling_rook(move);
    }
}

auto Bitboard::reset_castling_rook(const Move &move) -> void {
    if (move.from.file().file < move.to.file().file) {
        // Kingside castling
        clear_square(Square{File{'F'}, move.from.rank()});                                                         // remove rook
        set_piece(Piece{.type = PieceType::Rook, .color = move.piece.color}, Square{File{'H'}, move.from.rank()}); // place rook on h-file
    } else {
        // Queenside castling
        clear_square(Square{File{'D'}, move.from.rank()});                                                         // remove rook
        set_piece(Piece{.type = PieceType::Rook, .color = move.piece.color}, Square{File{'A'}, move.from.rank()}); // place rook on a-file
    }
}

auto Bitboard::all_legal_moves(
    [[maybe_unused]] const Color &side_to_move, [[maybe_unused]] const CastlingRights &castling_rights, [[maybe_unused]] const std::optional<Square> &en_passant_square
) const -> std::vector<Move> {
    return {};
}

} // namespace chesscore
