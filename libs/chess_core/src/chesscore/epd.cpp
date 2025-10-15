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

auto advance(const std::string &line, size_t &index) {
    while (index < line.length() && line[index] == ' ') {
        ++index;
    }
}

auto read_word(const std::string &line, size_t &index) -> std::string {
    advance(line, index);
    const auto start = index;
    while (index < line.length() && line[index] != ' ' && line[index] != ';') {
        ++index;
    }
    return line.substr(start, index - start);
}

auto read_int(const std::string &line, size_t &index, EpdRecord::opt_int &opt_int) -> void {
    advance(line, index);
    const auto start = index;
    while (index < line.length() && std::isdigit(line[index]) != 0) {
        ++index;
    }
    opt_int = std::stoi(line.substr(start, index - start));
}

auto read_move(const std::string &line, size_t &index, EpdRecord::opt_move &opt_move) -> void {
    advance(line, index);
    opt_move = read_word(line, index);
}

auto read_moves(const std::string &line, size_t &index, EpdRecord::move_list &moves) -> void {
    advance(line, index);
    while (index < line.length() && line[index] != ';') {
        moves.emplace_back(read_word(line, index));
        advance(line, index);
    }
    ++index;
}

auto check_operation_end(const std::string &line, size_t &index) -> void {
    advance(line, index);
    if (index >= line.length() || line[index] != ';') {
        throw InvalidEpd("Invalid operation end");
    }
    ++index;
}

auto get_string(const std::string &line, size_t &index) -> std::string {
    advance(line, index);
    if (index >= line.length() || line[index] != '"') {
        throw InvalidEpd("Invalid string");
    }
    ++index;
    const auto start = index;
    while (index < line.length() && line[index] != '"') {
        ++index;
    }
    return line.substr(start, index++ - start);
}

auto read_string(const std::string &line, size_t &index, EpdRecord::str_list &list, int list_index) -> void {
    list[list_index] = get_string(line, index);
}

auto read_string(const std::string &line, size_t &index, EpdRecord::opt_str &opt_str) -> void {
    opt_str = get_string(line, index);
}

auto collect_operands(const std::string &line, size_t &index, EpdRecord::str_list &list) -> void {
    advance(line, index);
    while (index < line.length() && line[index] != ';') {
        const auto operand = read_word(line, index);
        list.emplace_back(operand);
        advance(line, index);
    }
    ++index;
}

auto read_player_identifier(const std::string &line, size_t &index, EpdRecord::player_identifier &identifier) -> void {
    identifier.first = read_word(line, index);
    identifier.second = get_string(line, index);
}

auto read_operation(const std::string &line, size_t &index, EpdRecord &record) -> void {
    const auto opcode = read_word(line, index);
    if (opcode == "acd") {
        read_int(line, index, record.acd);
        check_operation_end(line, index);
    } else if (opcode == "acn") {
        read_int(line, index, record.acn);
        check_operation_end(line, index);
    } else if (opcode == "acs") {
        read_int(line, index, record.acs);
        check_operation_end(line, index);
    } else if (opcode == "bm") {
        read_moves(line, index, record.bm);
    } else if (opcode == "c0" || opcode == "c1" || opcode == "c2" || opcode == "c3" || opcode == "c4" || opcode == "c5" || opcode == "c6" || opcode == "c7" || opcode == "c8" ||
               opcode == "c9") {
        read_string(line, index, record.c, opcode[1] - '0');
        check_operation_end(line, index);
    } else if (opcode == "ce") {
        read_int(line, index, record.ce);
        check_operation_end(line, index);
    } else if (opcode == "dm") {
        read_int(line, index, record.dm);
        check_operation_end(line, index);
    } else if (opcode == "draw_accept;") {
        record.draw_accept = true;
        ++index;
    } else if (opcode == "draw_claim;") {
        record.draw_claim = true;
        ++index;
    } else if (opcode == "draw_offer;") {
        record.draw_offer = true;
        ++index;
    } else if (opcode == "draw_reject;") {
        record.draw_reject = true;
        ++index;
    } else if (opcode == "eco") {
        read_string(line, index, record.eco);
        check_operation_end(line, index);
    } else if (opcode == "fmvn") {
        read_int(line, index, record.fmvn);
        check_operation_end(line, index);
    } else if (opcode == "hmvc") {
        read_int(line, index, record.hmvc);
        check_operation_end(line, index);
    } else if (opcode == "id") {
        read_string(line, index, record.id);
        check_operation_end(line, index);
    } else if (opcode == "nic") {
        read_string(line, index, record.nic);
        check_operation_end(line, index);
    } else if (opcode == "noop") {
        collect_operands(line, index, record.noop_ops);
    } else if (opcode == "pm") {
        read_move(line, index, record.pm);
        check_operation_end(line, index);
    } else if (opcode == "pv") {
        read_moves(line, index, record.pv);
    } else if (opcode == "rc") {
        read_int(line, index, record.rc);
        check_operation_end(line, index);
    } else if (opcode == "resign;") {
        record.resign = true;
        ++index;
    } else if (opcode == "sm") {
        read_move(line, index, record.sm);
        check_operation_end(line, index);
    } else if (opcode == "tcgs") {
        read_int(line, index, record.tcgs);
        check_operation_end(line, index);
    } else if (opcode == "tcri") {
        read_player_identifier(line, index, record.tcri);
        check_operation_end(line, index);
    } else if (opcode == "tcsi") {
        read_player_identifier(line, index, record.tcsi);
        check_operation_end(line, index);
    } else if (opcode == "v0" || opcode == "v1" || opcode == "v2" || opcode == "v3" || opcode == "v4" || opcode == "v5" || opcode == "v6" || opcode == "v7" || opcode == "v8" ||
               opcode == "v9") {
        read_string(line, index, record.v, opcode[1] - '0');
        check_operation_end(line, index);
    }
}

} // namespace

auto parse_epd_line(const std::string &line) -> EpdRecord {
    auto [fen, index] = read_fen_fields(line);
    if (index < line.length()) {
        advance(line, index);
    }
    EpdRecord record;
    record.position = Position{fen};
    while (index < line.length()) {
        read_operation(line, index, record);
    }

    return record;
}

auto read_epd([[maybe_unused]] const std::string &path) -> std::vector<EpdRecord> {
    return {};
}

} // namespace chesscore
