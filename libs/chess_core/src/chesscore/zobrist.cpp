/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#include "chesscore/zobrist.h"
#include "chesscore/position.h"

namespace chesscore {

bool ZobristKeys::m_initialized{false};
std::array<ZobristKeys::key_t, 2 * piece_type_count * Square::count> ZobristKeys::m_piece_keys{};
std::array<ZobristKeys::key_t, 16> ZobristKeys::m_castling_keys;
std::array<ZobristKeys::key_t, Rank::max_rank> ZobristKeys::m_enpassant_keys;
ZobristKeys::key_t ZobristKeys::m_side_key;

std::optional<ZobristHash::key_t> ZobristHash::m_starting_position_hash{std::nullopt};

auto ZobristHash::from_position(const Position &position) -> ZobristHash {
    ZobristHash hash{};
    if (position.side_to_move() == Color::Black) {
        hash.swap_side();
    }
    if (position.en_passant_target().has_value()) {
        hash.set_enpassant(position.en_passant_target().value().rank());
    }
    hash.set_castling(position.castling_rights());
    Square square{Square::A1};
    for (int square_index = 0; square_index < Square::count; ++square_index) {
        const auto piece = position.board().get_piece(square);
        if (piece) {
            hash.set_piece(piece.value(), square);
        }
        square += 1;
    }
    return hash;
}

auto ZobristHash::starting_position_hash() -> ZobristHash {
    if (!m_starting_position_hash.has_value()) {
        ZobristHash hash{};
        const auto starting_position = Position{FenString::starting_position()};
        m_starting_position_hash = ZobristHash::from_position(starting_position).hash();
    }
    return ZobristHash{m_starting_position_hash.value()};
}

} // namespace chesscore
