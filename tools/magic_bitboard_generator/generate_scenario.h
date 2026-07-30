/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#ifndef MAGIC_BITBOARD_GENERATOR_GENERATE_SCENARIO_H
#define MAGIC_BITBOARD_GENERATOR_GENERATE_SCENARIO_H

#include "arg_parse.h"

#include "database.h"

#include <filesystem>
#include <fstream>
#include <string>

namespace magic_bitboard_generator::generate_scenario {

auto generate_headers(const Args &args) -> void;

class HeaderFile {
public:
    HeaderFile(const std::filesystem::path &out_path) : m_file{out_path} {}

    auto write_header() -> void;
    auto write_data_set(const std::string &piece_name, const std::vector<Record> &records) -> void;
    auto start_data_set(const std::string &piece_name) -> void;
    auto write_record(const Record &record) -> void;
    auto end_data_set() -> void;
    auto write_footer() -> void;
private:
    std::ofstream m_file;
};

} // namespace magic_bitboard_generator::generate_scenario

#endif
