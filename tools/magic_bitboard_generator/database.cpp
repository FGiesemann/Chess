/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#include "database.h"
#include <chesscore/chesscore.h>
#include <chesscore/piece.h>

auto Record::set_magics(const Magics &magics, const TableStats &stats) -> void {
    m_magics = magics;
    m_stats = stats;
    m_magics_found = true;
}

auto RecordWriter::write(const Record &record) -> void {
    // TODO
}

auto RecordReader::read() const -> Record {
    // TODO
    return Record{chesscore::PieceType::Rook, chesscore::Square::A1};
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
    // TODO
}

auto DatabaseReader::read() const -> Database {
    // TODO
    return Database{};
}
