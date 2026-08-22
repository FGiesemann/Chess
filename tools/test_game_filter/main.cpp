#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include <chesscore/epd.h>
#include <chessgame/pgn.h>

static constexpr size_t min_ply_count{40};
static constexpr float max_ply_percentage{0.6f};
static constexpr float min_ply_percentage{0.2f};

auto process_game(const chessgame::Game &game, std::ostream &output) -> void;
auto mainline_plys(const chessgame::Game &game) -> std::size_t;
auto random_depth(std::size_t total_ply_count) -> std::size_t;
auto play_moves(const chessgame::Game &game, std::size_t depth) -> chessgame::ConstCursor;

std::random_device rd;
std::mt19937 gen(rd());

auto main() -> int {
    std::string input_file_path{R"(D:\Programmierung\Projekte\Chess\data\PGN\collected_twic_1560-1615.pgn)"};
    std::string output_file_path{R"(D:\Programmierung\Projekte\Chess\data\PGN\collected_twic_1560-1615.epd)"};

    std::ifstream input_pgn{input_file_path};
    std::ofstream epd_file{output_file_path};
    auto parser = chessgame::PGNParser{input_pgn};

    auto game = parser.read_game();
    std::uint64_t count{0};
    while (game.has_value()) {
        ++count;
        process_game(game.value(), epd_file);
        std::cout << "Processed " << count << " games\n";

        try {
            game = parser.read_game();
        } catch (...) {
            std::cout << "Error. Skipping game.\n";
            parser.skip_to_next_game();
            game = parser.read_game();
        }
    }

    return 0;
}

auto process_game(const chessgame::Game &game, std::ostream &output) -> void {
    const auto total_plys = mainline_plys(game);
    if (total_plys < min_ply_count) {
        return;
    }
    const auto moves_to_play = random_depth(total_plys);
    const auto cursor = play_moves(game, moves_to_play);
    const auto position = cursor.position();

    chesscore::EpdRecord record;
    record.position = position;
    record.fmvn = position.fullmove_number();
    record.hmvc = position.halfmove_clock();
    chesscore::write_epd_record(output, record);
}

auto play_moves(const chessgame::Game &game, std::size_t depth) -> chessgame::ConstCursor {
    auto cursor = game.const_cursor();
    for (std::size_t i = 0; i < depth; ++i) {
        cursor = cursor.child(0).value();
    }
    return cursor;
}

auto mainline_plys(const chessgame::Game &game) -> std::size_t {
    std::size_t plys{0};
    auto cursor = game.const_cursor();
    while (cursor.child_count() > 0) {
        ++plys;
        cursor = cursor.child(0).value();
    }
    return plys;
}

auto random_depth(std::size_t total_ply_count) -> std::size_t {
    std::size_t max_depth = total_ply_count * max_ply_percentage;
    std::size_t min_depth = total_ply_count * min_ply_percentage;
    return std::uniform_int_distribution<std::size_t>(min_depth, max_depth)(gen);
}
