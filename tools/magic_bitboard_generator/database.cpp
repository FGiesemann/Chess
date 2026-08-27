/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#include "database.h"
#include <chess_core/chess_core.h>
#include <chess_core/piece.h>
#include <chess_core/square.h>

#include <format>
#include <fstream>

namespace magic_bitboard_generator {

auto Record::set_magics(const Magics &magics, const TableStats &stats) -> void {
    m_magics = magics;
    m_stats = stats;
    m_magics_found = true;
}

auto Record::update_magics(const Magics &magics, const TableStats &stats) -> bool {
    if (!m_magics_found) {
        set_magics(magics, stats);
        return true;
    }
    if (m_stats.max_index > stats.max_index) {
        set_magics(magics, stats);
        return true;
    }
    return false;
}

auto RecordWriter::write(const Record &record, std::ostream &ostream) -> void {
    ostream << std::format("{:c} {} {:1d}", chess_core::Piece{record.piece(), chess_core::Color::White}.piece_char(), to_string(record.square()), record.has_magics() ? 1 : 0);
    if (record.has_magics()) {
        ostream << std::format(
            " {:016x} {} {} {} {}", record.magics().magic_number, record.magics().shift, record.stats().max_index, record.stats().blocker_configs,
            record.stats().constructive_collisions
        );
    }
    ostream << '\n';
}

auto RecordReader::read(std::istream &istream) -> Record {
    unsigned char piece_char{};
    istream >> piece_char;
    if (piece_char != 'R' && piece_char != 'B') {
        throw chess_core::ChessException{"Invalid piece type"};
    }

    const auto piece = piece_char == 'R' ? chess_core::PieceType::Rook : chess_core::PieceType::Bishop;
    istream >> std::skipws;

    char file_char{};
    istream >> file_char;
    int rank{};
    istream >> rank;
    const chess_core::Square square{chess_core::File{file_char}, chess_core::Rank{rank}};

    Record record{piece, square};
    int has_magics{};
    istream >> has_magics;
    if (has_magics == 1) {
        std::uint64_t magic_number{};
        std::uint8_t shift{};
        istream >> std::hex >> magic_number >> std::dec >> shift;
        std::uint64_t max_index{};
        std::uint64_t blocker_configs{};
        std::uint64_t constructive_collisions{};
        istream >> max_index >> blocker_configs >> constructive_collisions;
        record.set_magics(
            Magics{.magic_number = magic_number, .shift = shift},
            TableStats{.blocker_configs = blocker_configs, .max_index = max_index, .constructive_collisions = constructive_collisions}
        );
    }

    return record;
}

Database::Database() {
    m_rook_records.reserve(64);
    m_bishop_records.reserve(64);
    for (chess_core::Square square = chess_core::Square::A1; square != chess_core::Square::H8; square += 1) {
        m_rook_records.emplace_back(chess_core::PieceType::Rook, square);
        m_bishop_records.emplace_back(chess_core::PieceType::Bishop, square);
    }
    m_rook_records.emplace_back(chess_core::PieceType::Rook, chess_core::Square::H8);
    m_bishop_records.emplace_back(chess_core::PieceType::Bishop, chess_core::Square::H8);
}

auto Database::record(chess_core::PieceType piece, const chess_core::Square &square) const -> const Record & {
    if (piece == chess_core::PieceType::Rook) {
        return m_rook_records[square.index()];
    }
    if (piece == chess_core::PieceType::Bishop) {
        return m_bishop_records[square.index()];
    }
    throw chess_core::ChessException{"Invalid piece type"};
}

auto Database::record(chess_core::PieceType piece, const chess_core::Square &square) -> Record & {
    if (piece == chess_core::PieceType::Rook) {
        return m_rook_records[square.index()];
    }
    if (piece == chess_core::PieceType::Bishop) {
        return m_bishop_records[square.index()];
    }
    throw chess_core::ChessException{"Invalid piece type"};
}

auto DatabaseWriter::write(const Database &database) -> void {
    std::ofstream file{m_path};
    for (const auto &record : database.rook_records()) {
        RecordWriter::write(record, file);
    }
    for (const auto &record : database.bishop_records()) {
        RecordWriter::write(record, file);
    }
}

auto DatabaseReader::read() const -> Database {
    Database database{};
    std::ifstream file{m_path};
    for (auto &record : database.rook_records()) {
        record = RecordReader::read(file);
    }
    for (auto &record : database.bishop_records()) {
        record = RecordReader::read(file);
    }
    return database;
}

} // namespace magic_bitboard_generator
