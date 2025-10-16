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

auto read_string(const std::string &line, size_t &index, EpdRecord::str_list &list, size_t list_index) -> void {
    list[list_index] = get_string(line, index);
}

auto read_string(const std::string &line, size_t &index, EpdRecord::opt_str &opt_str) -> void {
    opt_str = get_string(line, index);
}

auto get_word_or_string(const std::string &line, size_t &index) -> std::string {
    if (line[index] == '"') {
        return get_string(line, index);
    }
    return read_word(line, index);
}

template<typename ListT>
auto collect_operands(const std::string &line, size_t &index, ListT &list) -> void {
    advance(line, index);
    while (index < line.length() && line[index] != ';') {
        const auto operand = get_word_or_string(line, index);
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
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_int(in_line, in_index, in_record.acd);
                 check_operation_end(in_line, in_index);
             }},
            {"acn",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_int(in_line, in_index, in_record.acn);
                 check_operation_end(in_line, in_index);
             }},
            {"acs",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_int(in_line, in_index, in_record.acs);
                 check_operation_end(in_line, in_index);
             }},
            {"bm", [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) { read_moves(in_line, in_index, in_record.bm); }},
            {"c0",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.c, 0);
                 check_operation_end(in_line, in_index);
             }},
            {"c1",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.c, 1);
                 check_operation_end(in_line, in_index);
             }},
            {"c2",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.c, 2);
                 check_operation_end(in_line, in_index);
             }},
            {"c3",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.c, 3);
                 check_operation_end(in_line, in_index);
             }},
            {"c4",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.c, 4);
                 check_operation_end(in_line, in_index);
             }},
            {"c5",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.c, 5);
                 check_operation_end(in_line, in_index);
             }},
            {"c6",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.c, 6);
                 check_operation_end(in_line, in_index);
             }},
            {"c7",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.c, 7);
                 check_operation_end(in_line, in_index);
             }},
            {"c8",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.c, 8);
                 check_operation_end(in_line, in_index);
             }},
            {"c9",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.c, 9);
                 check_operation_end(in_line, in_index);
             }},
            {"ce",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_int(in_line, in_index, in_record.ce);
                 check_operation_end(in_line, in_index);
             }},
            {"dm",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_int(in_line, in_index, in_record.dm);
                 check_operation_end(in_line, in_index);
             }},
            {"draw_accept",
             [](EpdRecord &in_record, const std::string &, size_t &in_index) {
                 in_record.draw_accept = true;
                 ++in_index;
             }},
            {"draw_claim",
             [](EpdRecord &in_record, const std::string &, size_t &in_index) {
                 in_record.draw_claim = true;
                 ++in_index;
             }},
            {"draw_offer",
             [](EpdRecord &in_record, const std::string &, size_t &in_index) {
                 in_record.draw_offer = true;
                 ++in_index;
             }},
            {"draw_reject",
             [](EpdRecord &in_record, const std::string &, size_t &in_index) {
                 in_record.draw_reject = true;
                 ++in_index;
             }},
            {"eco",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.eco);
                 check_operation_end(in_line, in_index);
             }},
            {"fmvn",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_int(in_line, in_index, in_record.fmvn);
                 check_operation_end(in_line, in_index);
             }},
            {"hmvc",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_int(in_line, in_index, in_record.hmvc);
                 check_operation_end(in_line, in_index);
             }},
            {"id",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.id);
                 check_operation_end(in_line, in_index);
             }},
            {"nic",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.nic);
                 check_operation_end(in_line, in_index);
             }},
            {"noop", [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) { collect_operands(in_line, in_index, in_record.noop_ops); }},
            {"pm",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_move(in_line, in_index, in_record.pm);
                 check_operation_end(in_line, in_index);
             }},
            {"pv", [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) { read_moves(in_line, in_index, in_record.pv); }},
            {"rc",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_int(in_line, in_index, in_record.rc);
                 check_operation_end(in_line, in_index);
             }},
            {"resign",
             [](EpdRecord &in_record, const std::string &, size_t &in_index) {
                 in_record.resign = true;
                 ++in_index;
             }},
            {"sm",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_move(in_line, in_index, in_record.sm);
                 check_operation_end(in_line, in_index);
             }},
            {"tcgs",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_int(in_line, in_index, in_record.tcgs);
                 check_operation_end(in_line, in_index);
             }},
            {"tcri",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_player_identifier(in_line, in_index, in_record.tcri);
                 check_operation_end(in_line, in_index);
             }},
            {"tcsi",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_player_identifier(in_line, in_index, in_record.tcsi);
                 check_operation_end(in_line, in_index);
             }},
            {"v0",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.v, 0);
                 check_operation_end(in_line, in_index);
             }},
            {"v1",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.v, 1);
                 check_operation_end(in_line, in_index);
             }},
            {"v2",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.v, 2);
                 check_operation_end(in_line, in_index);
             }},
            {"v3",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.v, 3);
                 check_operation_end(in_line, in_index);
             }},
            {"v4",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.v, 4);
                 check_operation_end(in_line, in_index);
             }},
            {"v5",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.v, 5);
                 check_operation_end(in_line, in_index);
             }},
            {"v6",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.v, 6);
                 check_operation_end(in_line, in_index);
             }},
            {"v7",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.v, 7);
                 check_operation_end(in_line, in_index);
             }},
            {"v8",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.v, 8);
                 check_operation_end(in_line, in_index);
             }},
            {"v9",
             [](EpdRecord &in_record, const std::string &in_line, size_t &in_index) {
                 read_string(in_line, in_index, in_record.v, 9);
                 check_operation_end(in_line, in_index);
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
    try {
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
    } catch (InvalidFen &error) {
        throw InvalidEpd{std::string{"Error reading EPD: "} + error.what()};
    }
}

auto read_epd(std::istream &input) -> std::vector<EpdRecord> {
    std::vector<EpdRecord> records;
    std::string line{};
    int line_number{0};
    try {
        while (std::getline(input, line)) {
            ++line_number;
            if (line.empty() || line[0] == '#') {
                continue;
            }
            records.emplace_back(parse_epd_line(line));
        }
    } catch (InvalidEpd &error) {
        throw InvalidEpd{std::string{"Error in line "} + std::to_string(line_number) + ": " + error.what()};
    }

    return records;
}

} // namespace chesscore
