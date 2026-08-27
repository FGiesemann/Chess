/* ************************************************************************** *
 * Chess Engine Maat                                                          *
 * Chess playing engine                                                       *
 * ************************************************************************** */

#include "chess_engine/uci_adapter.h"

namespace chess_engine {

auto construct_position(const chess_uci::position_command &command) -> std::pair<chess_core::Position, UCIMoveList> {
    UCIMoveList move_list{};
    const auto fen = (command.fen == chess_uci::position_command::startpos) ? chess_core::FenString::starting_position() : chess_core::FenString{command.fen};
    auto position = chess_core::Position{fen};
    std::ranges::for_each(command.moves, [&position, &move_list](const chess_uci::UCIMove &move) -> void {
        const auto matched_move = chess_uci::convert_legal_move(move, position);
        if (matched_move.has_value()) {
            position.make_move(matched_move.value());
            move_list.push_back(move);
        } else {
            throw chess_uci::UCIError{"Invalid move " + to_string(move)};
        }
    });
    return std::make_pair(position, move_list);
}

} // namespace chess_engine
