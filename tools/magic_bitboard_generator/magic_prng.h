/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#ifndef MAGIC_BITBOARD_GENERATOR_MAGIC_PRNG_H
#define MAGIC_BITBOARD_GENERATOR_MAGIC_PRNG_H

#include <random>

namespace magic_bitboard_generator {

class MagicPRNG {
public:
    MagicPRNG() = default;
    MagicPRNG(std::uint64_t seed) : m_engine{seed} {}

    auto seed(std::uint64_t seed) -> void { m_engine.seed(seed); }
    auto get() -> std::uint64_t;
private:
    std::mt19937_64 m_engine;
};

} // namespace magic_bitboard_generator

#endif
