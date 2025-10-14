/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/epd.h"

namespace chesscore {

namespace {

auto read_fen_fields(const std::string &line) -> std::pair<FenString, size_t> {
    const auto place = detail::check_piece_placement(line);
    const auto piece_placement = place.first;
    const auto side = detail::check_side_to_move(line, place.second);
    const auto side_to_move = side.first;
    const auto castling = detail::check_castling_availability(line, side.second);
    const auto castling_rights = castling.first;
    const auto en_p = detail::check_en_passant_target_square(line, side_to_move, castling.second);
    const auto en_passant = en_p.first;
    FenString fen{piece_placement, side_to_move, castling_rights, en_passant, 0, 1};
    return std::make_pair(fen, en_p.second);
}

} // namespace

auto parse_epd_line(const std::string &line) -> EpdRecord {
    const auto [fen, index] = read_fen_fields(line);
    EpdRecord record;
    record.position = Position{fen};
    return record;
}

auto read_epd([[maybe_unused]] const std::string &path) -> std::vector<EpdRecord> {
    return {};
}

} // namespace chesscore
