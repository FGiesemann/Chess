/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#include "database.h"
#include <chesscore/chesscore.h>
#include <chesscore/piece.h>
#include <chesscore/square.h>

#include <format>
#include <fstream>

auto Record::set_magics(const Magics &magics, const TableStats &stats) -> void {
    m_magics = magics;
    m_stats = stats;
    m_magics_found = true;
}

auto RecordWriter::write(const Record &record, std::ostream &ostream) -> void {
    ostream << std::format("{:c} {} {:1d}",
                           chesscore::Piece{.type = record.piece(), .color = chesscore::Color::White}.piece_char(),
                           to_string(record.square()), record.has_magics() ? 1 : 0);
    if (record.has_magics()) {
        ostream << std::format(" {:016x} {} {} {}", record.magics().magic_number, record.magics().shift,
                               record.stats().max_index, record.stats().constructive_collisions);
    }
    ostream << '\n';
}

auto RecordReader::read(std::istream &istream) -> Record {
    unsigned char piece_char{};
    istream >> piece_char;
    if (piece_char != 'R' && piece_char != 'B') {
        throw chesscore::ChessException{"Invalid piece type"};
    }

    const auto piece = piece_char == 'R' ? chesscore::PieceType::Rook : chesscore::PieceType::Bishop;
    istream >> std::skipws;

    unsigned char file_char{};
    istream >> file_char;
    int rank{};
    istream >> rank;

    const chesscore::Square square{chesscore::File{file_char}, chesscore::Rank{rank}};

    Record record{piece, square};
    int has_magics{};
    istream >> has_magics;
    if (has_magics == 1) {
        std::uint64_t magic_number{};
        std::uint64_t shift{};
        istream >> std::hex >> magic_number >> std::dec >> shift;
        std::uint64_t max_index{};
        std::uint64_t constructive_collisions{};
        istream >> max_index >> constructive_collisions;
        record.set_magics(Magics{.magic_number = magic_number, .shift = shift},
                          TableStats{.max_index = max_index, .constructive_collisions = constructive_collisions});
    }

    return record;
}

Database::Database() {
    m_rook_records.reserve(64);
    m_bishop_records.reserve(64);
    for (chesscore::Square square = chesscore::Square::A1; square != chesscore::Square::H8; square += 1) {
        m_rook_records.emplace_back(chesscore::PieceType::Rook, square);
        m_bishop_records.emplace_back(chesscore::PieceType::Bishop, square);
    }
    m_rook_records.emplace_back(chesscore::PieceType::Rook, chesscore::Square::H8);
    m_bishop_records.emplace_back(chesscore::PieceType::Bishop, chesscore::Square::H8);
}

auto Database::record(chesscore::PieceType piece, const chesscore::Square &square) const -> const Record & {
    if (piece == chesscore::PieceType::Rook) {
        return m_rook_records[square.index()];
    }
    if (piece == chesscore::PieceType::Bishop) {
        return m_bishop_records[square.index()];
    }
    throw chesscore::ChessException{"Invalid piece type"};
}

auto Database::record(chesscore::PieceType piece, const chesscore::Square &square) -> Record & {
    if (piece == chesscore::PieceType::Rook) {
        return m_rook_records[square.index()];
    }
    if (piece == chesscore::PieceType::Bishop) {
        return m_bishop_records[square.index()];
    }
    throw chesscore::ChessException{"Invalid piece type"};
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
