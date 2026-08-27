/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESS_CORE_ZOBRIST_H
#define CHESS_CORE_ZOBRIST_H

#include "chess_core/piece.h"
#include "chess_core/position_types.h"
#include "chess_core/square.h"

#include <array>
#include <cstdint>
#include <optional>
#include <random>

namespace chess_core {

class Position;

class ZobristKeys {
public:
    using key_t = std::uint64_t;
    static constexpr std::mt19937_64::result_type RandSeed{3275739884ULL};

    static auto initialize() -> void {
        if (m_initialized) {
            return;
        }
        std::mt19937_64 rng{RandSeed};
        std::uniform_int_distribution<key_t> dist;
        for (size_t i = 0; i < m_piece_keys.size(); ++i) {
            m_piece_keys[i] = generate_nonzero_key(rng, dist);
        }
        for (size_t i = 0; i < m_castling_keys.size(); ++i) {
            m_castling_keys[i] = generate_nonzero_key(rng, dist);
        }
        for (size_t i = 0; i < m_enpassant_keys.size(); ++i) {
            m_enpassant_keys[i] = generate_nonzero_key(rng, dist);
        }
        m_side_key = generate_nonzero_key(rng, dist);
        m_initialized = true;
    }

    static auto initialized() -> bool { return m_initialized; }

    static auto piece_key(Piece piece, Square square) -> key_t { return m_piece_keys[piece_index(piece, square)]; }
    static auto piece_key(PieceType type, Color color, Square square) -> key_t { return piece_key(Piece{type, color}, square); }
    static auto castling_key(CastlingRights rights) -> key_t { return m_castling_keys[castling_index(rights)]; }
    static auto enpassant_key(File file) -> key_t { return m_enpassant_keys[static_cast<size_t>(file.file)]; }
    static auto side_key() -> key_t { return m_side_key; }
private:
    static bool m_initialized;
    static std::array<key_t, 2 * piece_type_count * Square::count> m_piece_keys;
    static std::array<key_t, CastlingRights::max_combinations> m_castling_keys;
    static std::array<key_t, Rank::count> m_enpassant_keys;
    static key_t m_side_key; // black to move

    static auto generate_nonzero_key(std::mt19937_64 &rng, std::uniform_int_distribution<key_t> &dist) -> key_t {
        key_t key = dist(rng);
        while (key == 0) {
            key = dist(rng);
        }
        return key;
    }

    static auto piece_index(Piece piece, Square square) -> std::size_t {
        size_t index = piece.color() == Color::White ? 0 : piece_type_count * Square::count;
        return index + get_index(piece.type()) * Square::count + square.index();
    }

    static auto castling_index(CastlingRights rights) -> std::size_t {
        static constexpr std::size_t bit_0 = 1;
        static constexpr std::size_t bit_1 = 2;
        static constexpr std::size_t bit_2 = 4;
        static constexpr std::size_t bit_3 = 8;

        std::size_t index = rights.can_black_queen() ? bit_0 : 0;
        index += rights.can_black_king() ? bit_1 : 0;
        index += rights.can_white_queen() ? bit_2 : 0;
        index += rights.can_white_king() ? bit_3 : 0;
        return index;
    }
};

class ZobristHash {
public:
    using key_t = ZobristKeys::key_t;

    ZobristHash() { ZobristKeys::initialize(); }
    ZobristHash(key_t hash_value) : ZobristHash() { m_hash = hash_value; }

    static auto from_position(const Position &position) -> ZobristHash;
    static auto starting_position_hash() -> ZobristHash;

    auto hash() const -> key_t { return m_hash; }

    auto set_piece(Piece piece, Square square) -> ZobristHash & {
        m_hash ^= ZobristKeys::piece_key(piece, square);
        return *this;
    }
    auto clear_piece(Piece piece, Square square) -> ZobristHash & { return set_piece(piece, square); }
    auto move_piece(Piece piece, Square from, Square to) -> ZobristHash & {
        clear_piece(piece, from);
        set_piece(piece, to);
        return *this;
    }
    auto swap_side() -> ZobristHash & {
        m_hash ^= ZobristKeys::side_key();
        return *this;
    }
    auto clear_enpassant(File file) -> ZobristHash & {
        m_hash ^= ZobristKeys::enpassant_key(file);
        return *this;
    }
    auto set_enpassant(File file) -> ZobristHash & {
        m_hash ^= ZobristKeys::enpassant_key(file);
        return *this;
    }
    auto set_castling(CastlingRights rights) -> ZobristHash & {
        m_hash ^= ZobristKeys::castling_key(rights);
        return *this;
    }
    auto switch_castling(CastlingRights before, CastlingRights after) -> ZobristHash & {
        m_hash ^= ZobristKeys::castling_key(before);
        m_hash ^= ZobristKeys::castling_key(after);
        return *this;
    }

    auto operator==(const ZobristHash &rhs) const -> bool { return m_hash == rhs.m_hash; }
private:
    key_t m_hash{0};
    static std::optional<key_t> m_starting_position_hash;
};

} // namespace chess_core

#endif
