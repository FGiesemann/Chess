/* ************************************************************************** *
 * Chess Tools - Magic Bitboard Generator                                     *
 * Search for magic numbers                                                   *
 * ************************************************************************** */

#ifndef MAGIC_BITBOARD_DATABASE_H
#define MAGIC_BITBOARD_DATABASE_H

#include <chesscore/piece.h>
#include <chesscore/square.h>

#include "magic_bitboard.h"

#include <filesystem>
#include <vector>

struct TableStats {
    std::uint64_t max_index{};
    std::uint64_t constructive_collisions{};
};

class Record {
public:
    Record(chesscore::PieceType piece, chesscore::Square square) : m_piece(piece), m_square(square) {}

    [[nodiscard]] auto piece() const -> chesscore::PieceType { return m_piece; }
    [[nodiscard]] auto square() const -> chesscore::Square { return m_square; }
    [[nodiscard]] auto has_magics() const -> bool { return m_magics_found; }
    [[nodiscard]] auto magics() const -> const Magics & { return m_magics; }
    [[nodiscard]] auto stats() const -> const TableStats & { return m_stats; }

    auto set_magics(const Magics &magics, const TableStats &stats) -> void;
private:
    chesscore::PieceType m_piece;
    chesscore::Square m_square;
    bool m_magics_found{false};

    Magics m_magics{};
    TableStats m_stats{};
};

class RecordWriter {
public:
    explicit RecordWriter(std::filesystem::path path) : m_path{std::move(path)} {}

    [[nodiscard]] auto path() const -> const std::filesystem::path & { return m_path; }

    auto write(const Record &record) -> void;
private:
    std::filesystem::path m_path;
};

class RecordReader {
public:
    explicit RecordReader(std::filesystem::path path) : m_path{std::move(path)} {}

    [[nodiscard]] auto read() const -> Record;
private:
    std::filesystem::path m_path;
};

class Database {
public:
    Database();

    [[nodiscard]] auto record(chesscore::PieceType piece, const chesscore::Square &square) const -> const Record &;
    [[nodiscard]] auto record(chesscore::PieceType piece, const chesscore::Square &square) -> Record &;
private:
    std::vector<Record> m_rook_records;
    std::vector<Record> m_bishop_records;
};

class DatabaseWriter {
public:
    explicit DatabaseWriter(std::filesystem::path path) : m_path{std::move(path)} {}

    auto write(const Database &database) -> void;
private:
    std::filesystem::path m_path;
};

class DatabaseReader {
public:
    explicit DatabaseReader(std::filesystem::path path) : m_path{std::move(path)} {}

    [[nodiscard]] auto read() const -> Database;
private:
    std::filesystem::path m_path;
};

#endif
