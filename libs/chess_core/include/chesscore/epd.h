/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_EPD_H
#define CHESSCORE_EPD_H

#include <optional>
#include <string>
#include <vector>

#include "chesscore/position.h"

namespace chesscore {

/**
 * \brief Exception thrown when an invalid EPD record is encountered.
 *
 */
class InvalidEpd : public ChessException {
public:
    /**
     * \brief Create an exeption with a message.
     *
     * \param message The message.
     */
    InvalidEpd(const std::string &message) : ChessException("Invalid EPD record: " + message) {}
};

/**
 * \brief An EPD record.
 *
 * A record describing a position in an EPD file.
 */
struct EpdRecord {
    using opt_int = std::optional<int>;                            ///< An optional integer.
    using opt_str = std::optional<std::string>;                    ///< An optional string.
    using str_list = std::vector<opt_str>;                         ///< List of optional strings.
    using san_str = std::string;                                   ///< String representing a move in SAN.
    using opt_move = std::optional<san_str>;                       ///< An optional move
    using move_list = std::vector<san_str>;                        ///< A (possibly empty) list of moves.
    using player_identifier = std::pair<std::string, std::string>; ///< Identifier (email and name) of a player.

    Position position;         ///< The position described by the record.
    opt_int acd;               ///< analysis count: depth.
    opt_int acn;               ///< analysis count: nodes.
    opt_int acs;               ///< analysis count: seconds.
    move_list bm;              ///< best moves.
    str_list c = str_list(10); ///< comments.
    opt_int ce;                ///< centipawn evaluation.
    opt_int dm;                ///< direct mate.
    bool draw_accept;          ///< draw accept.
    bool draw_claim;           ///< draw claim.
    bool draw_offer;           ///< draw offer.
    bool draw_reject;          ///< draw reject.
    opt_str eco;               ///< ECO code.
    opt_int fmvn;              ///< fullmove number.
    opt_int hmvc;              ///< halfmove clock.
    opt_str id;                ///< id.
    opt_str nic;               ///< New In Chess opening code.
    str_list noop_ops;         ///< noop operands.
    opt_move pm;               ///< predicted move.
    move_list pv;              ///< predicted variations.
    opt_int rc;                ///< repetition count.
    bool resign;               ///< resign.
    opt_move sm;               ///< supplied move.
    opt_int tcgs;              ///< telecommunication: game selector.
    player_identifier tcri;    ///< telecommunication: receiver identification.
    player_identifier tcsi;    ///< telecommunication: sender identification.
    str_list v = str_list(10); ///< variation names.
};

auto parse_epd_line(const std::string &line) -> EpdRecord;
auto read_epd(const std::string &path) -> std::vector<EpdRecord>;

} // namespace chesscore

#endif
