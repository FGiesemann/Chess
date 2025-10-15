/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/epd.h"

#include <istream>
#include <unordered_map>

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
    static const std::unordered_map < std::string_view,
        void (*)(EpdRecord &, const std::string &, size_t &)> operation_handlers = {
            {"acd",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_int(line, index, record.acd);
                 check_operation_end(line, index);
             }},
            {"acn",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_int(line, index, record.acn);
                 check_operation_end(line, index);
             }},
            {"acs",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_int(line, index, record.acs);
                 check_operation_end(line, index);
             }},
            {"bm", [](EpdRecord &record, const std::string &line, size_t &index) { read_moves(line, index, record.bm); }},
            {"c0",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.c, 0);
                 check_operation_end(line, index);
             }},
            {"c1",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.c, 1);
                 check_operation_end(line, index);
             }},
            {"c2",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.c, 2);
                 check_operation_end(line, index);
             }},
            {"c3",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.c, 3);
                 check_operation_end(line, index);
             }},
            {"c4",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.c, 4);
                 check_operation_end(line, index);
             }},
            {"c5",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.c, 5);
                 check_operation_end(line, index);
             }},
            {"c6",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.c, 6);
                 check_operation_end(line, index);
             }},
            {"c7",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.c, 7);
                 check_operation_end(line, index);
             }},
            {"c8",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.c, 8);
                 check_operation_end(line, index);
             }},
            {"c9",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.c, 9);
                 check_operation_end(line, index);
             }},
            {"ce",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_int(line, index, record.ce);
                 check_operation_end(line, index);
             }},
            {"dm",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_int(line, index, record.dm);
                 check_operation_end(line, index);
             }},
            {"draw_accept",
             [](EpdRecord &record, const std::string &, size_t &index) {
                 record.draw_accept = true;
                 ++index;
             }},
            {"draw_claim",
             [](EpdRecord &record, const std::string &, size_t &index) {
                 record.draw_claim = true;
                 ++index;
             }},
            {"draw_offer",
             [](EpdRecord &record, const std::string &, size_t &index) {
                 record.draw_offer = true;
                 ++index;
             }},
            {"draw_reject",
             [](EpdRecord &record, const std::string &, size_t &index) {
                 record.draw_reject = true;
                 ++index;
             }},
            {"eco",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.eco);
                 check_operation_end(line, index);
             }},
            {"fmvn",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_int(line, index, record.fmvn);
                 check_operation_end(line, index);
             }},
            {"hmvc",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_int(line, index, record.hmvc);
                 check_operation_end(line, index);
             }},
            {"id",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.id);
                 check_operation_end(line, index);
             }},
            {"nic",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.nic);
                 check_operation_end(line, index);
             }},
            {"noop", [](EpdRecord &record, const std::string &line, size_t &index) { collect_operands(line, index, record.noop_ops); }},
            {"pm",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_move(line, index, record.pm);
                 check_operation_end(line, index);
             }},
            {"pv", [](EpdRecord &record, const std::string &line, size_t &index) { read_moves(line, index, record.pv); }},
            {"rc",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_int(line, index, record.rc);
                 check_operation_end(line, index);
             }},
            {"resign",
             [](EpdRecord &record, const std::string &, size_t &index) {
                 record.resign = true;
                 ++index;
             }},
            {"sm",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_move(line, index, record.sm);
                 check_operation_end(line, index);
             }},
            {"tcgs",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_int(line, index, record.tcgs);
                 check_operation_end(line, index);
             }},
            {"tcri",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_player_identifier(line, index, record.tcri);
                 check_operation_end(line, index);
             }},
            {"tcsi",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_player_identifier(line, index, record.tcsi);
                 check_operation_end(line, index);
             }},
            {"v0",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.v, 0);
                 check_operation_end(line, index);
             }},
            {"v1",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.v, 1);
                 check_operation_end(line, index);
             }},
            {"v2",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.v, 2);
                 check_operation_end(line, index);
             }},
            {"v3",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.v, 3);
                 check_operation_end(line, index);
             }},
            {"v4",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.v, 4);
                 check_operation_end(line, index);
             }},
            {"v5",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.v, 5);
                 check_operation_end(line, index);
             }},
            {"v6",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.v, 6);
                 check_operation_end(line, index);
             }},
            {"v7",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.v, 7);
                 check_operation_end(line, index);
             }},
            {"v8",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.v, 8);
                 check_operation_end(line, index);
             }},
            {"v9",
             [](EpdRecord &record, const std::string &line, size_t &index) {
                 read_string(line, index, record.v, 9);
                 check_operation_end(line, index);
             }},
        };

    if (operation_handlers.contains(opcode)) {
        operation_handlers.at(opcode)(record, line, index);
    } else {
        EpdRecord::unknown_command unknown;
        unknown.opcode = opcode;
        collect_operands(line, index, unknown.operands);
        record.unknown_commands.emplace_back(std::move(unknown));
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

auto read_epd(std::istream &input) -> std::vector<EpdRecord> {
    std::vector<EpdRecord> records;
    std::string line{};
    while (std::getline(input, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        records.emplace_back(parse_epd_line(line));
    }

    return records;
}

} // namespace chesscore
