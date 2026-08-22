/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#ifndef MAGIC_BITBOARD_GENERATOR_MAGIC_PRNG_H
#define MAGIC_BITBOARD_GENERATOR_MAGIC_PRNG_H

#include <cstdint>

namespace magic_bitboard_generator {

class Xorshift64 {
public:
    Xorshift64() : m_state{88172645463325252UL} {}
    Xorshift64(std::uint64_t seed) : m_state{seed} {}
    auto seed(std::uint64_t seed) -> void { m_state = seed; }

    auto operator()() -> std::uint64_t {
        m_state ^= m_state << 13;
        m_state ^= m_state >> 7;
        m_state ^= m_state << 17;
        return m_state;
    }
private:
    std::uint64_t m_state;
};

template<typename T = Xorshift64>
class MagicPRNG {
public:
    MagicPRNG() = default;
    MagicPRNG(std::uint64_t seed) : m_engine{seed} {}

    auto seed(std::uint64_t seed) -> void { m_engine.seed(seed); }
    auto get() -> std::uint64_t { return m_engine() & m_engine() & m_engine(); }
private:
    T m_engine;
};

} // namespace magic_bitboard_generator

#endif
