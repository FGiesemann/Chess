/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#include "magic_prng.h"

namespace magic_bitboard_generator {

auto MagicPRNG::get() -> std::uint64_t {
    return m_engine() & m_engine() & m_engine();
}

} // namespace magic_bitboard_generator
