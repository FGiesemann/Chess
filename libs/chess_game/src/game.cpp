/* ************************************************************************** *
 * Chess Game                                                                 *
 * Representation of a single game of chess                                   *
 * ************************************************************************** */

#include "chess_game/game.h"

namespace chess_game {

Game::Game(const GameMetadata &metadata) : m_metadata{metadata} {
    m_root = std::make_shared<GameNode>(NodeId{1});
    const auto fen_tag = std::ranges::find_if(metadata, [](const auto &tag) { return tag.name == "FEN"; });
    const auto initial_fen = fen_tag == metadata.end() ? chess_core::FenString::starting_position() : chess_core::FenString{fen_tag->value};
    m_root->set_position(chess_core::Position{initial_fen});
}

Game::Game() : Game{GameMetadata{}} {}

auto Game::add_node(const std::shared_ptr<GameNode> &parent, const chess_core::Move &move) -> std::shared_ptr<GameNode> {
    auto child = std::make_shared<GameNode>(m_next_id++, move, parent);
    auto added_child = parent->append_child(child);
    if (added_child != child) {
        ++m_next_id;
    }
    return added_child;
}

} // namespace chess_game
