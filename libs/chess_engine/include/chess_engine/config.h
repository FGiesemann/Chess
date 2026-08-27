/* ************************************************************************** *
 * Chess Engine Maat                                                          *
 * Chess playing engine                                                       *
 * ************************************************************************** */

#ifndef CHESS_ENGINE_CONFIG_H
#define CHESS_ENGINE_CONFIG_H

#include <array>
#include <filesystem>

#include "chess_engine/types.h"

namespace chess_engine {

/**
 * \brief Configuration parameters for the search algorithm.
 */
struct MinimaxConfig {
    bool use_alpha_beta_pruning{true}; ///< If alpha-beta-pruning should be applied.
    bool use_move_ordering{true};      ///< If move ordering should be used.
};

/**
 * \brief Configuration parameters for the search strategy.
 */
struct SearchConfig {
    bool iterative_deepening{false}; ///< If iterative deepening should be used.
    bool search_pv_first{true};      ///< If the principal variation from the previous iteration should be searched first.
};

/**
 * \brief A table giving scores for pieces on squares.
 */
struct PieceSquareTable {
    /**
     * \brief Values for a piece on the squares of the board.
     *
     * The values are indexed according to chess_core::Square::index, i.e., entry
     * `values[0]` is for square a1, `values[1]` for square b1, and so on.
     */
    std::array<Score, chess_core::Square::count> values;

    /**
     * \brief Get the value for a given square.
     *
     * \param square The square.
     * \return The value for that square.
     */
    auto value(const chess_core::Square &square) const -> const Score & { return values[square.index()]; }

    /**
     * \brief Get the value for a given square.
     *
     * Returns a reference into the table that allows modifying the value.
     * \param square The square.
     * \return The value for that square.
     */
    auto value(const chess_core::Square &square) -> Score & { return values[square.index()]; }
};

/**
 * \brief Configuration for the evaluator.
 *
 * The configuration defines several parameters that influence the evaluation
 * of a chess position.
 */
class EvaluatorConfig {
public:
    bool use_material_balance{true};    ///< Count material balance in position evaluation.
    bool use_piece_square_tables{true}; ///< Use piece-square tables in position and move evaluation.
    bool use_promotion_bonus{true};     ///< Use additional bonus for pawn promotions in move evaluation.
    bool use_capture_bonus{false};      ///< Use additional bonus for captures in move evaluation.

    /**
     * \brief The scores for each piece type.
     *
     * The values have to be given in the same order as the piece types in the
     * chess_core::PieceType enumeration.
     */
    Score piece_values[6]{Score{100}, Score{300}, Score{300}, Score{500}, Score{900}, Score{0}};

    /**
     * \brief Scores for a piece on a square.
     *
     * The tables are defined from the perspective of the white player. For
     * black, the ranks have to be mirrored.
     */
    PieceSquareTable piece_square_tables[7]{
        // clang-format off
        // Pawn
        {
            Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{  0},
            Score{  5}, Score{ 10}, Score{ 10}, Score{-20}, Score{-20}, Score{ 10}, Score{ 10}, Score{  5},
            Score{  5}, Score{ -5}, Score{-10}, Score{  0}, Score{  0}, Score{-10}, Score{ -5}, Score{  5},
            Score{  0}, Score{  0}, Score{  0}, Score{ 20}, Score{ 20}, Score{  0}, Score{  0}, Score{  0},
            Score{  5}, Score{  5}, Score{ 10}, Score{ 25}, Score{ 25}, Score{ 10}, Score{  5}, Score{  5},
            Score{ 10}, Score{ 10}, Score{ 20}, Score{ 30}, Score{ 30}, Score{ 20}, Score{ 10}, Score{ 10},
            Score{ 50}, Score{ 50}, Score{ 50}, Score{ 50}, Score{ 50}, Score{ 50}, Score{ 50}, Score{ 50},
            Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{  0}
        }, 
        // Knight
        {
            Score{-50}, Score{-40}, Score{-30}, Score{-30}, Score{-30}, Score{-30}, Score{-40}, Score{-50},
            Score{-40}, Score{-20}, Score{  0}, Score{  5}, Score{  5}, Score{  0}, Score{-20}, Score{-40},
            Score{-30}, Score{  5}, Score{ 10}, Score{ 15}, Score{ 15}, Score{ 10}, Score{  5}, Score{-30},
            Score{-30}, Score{  0}, Score{ 15}, Score{ 20}, Score{ 20}, Score{ 15}, Score{  0}, Score{-30},
            Score{-30}, Score{  5}, Score{ 15}, Score{ 20}, Score{ 20}, Score{ 15}, Score{  5}, Score{-30},
            Score{-30}, Score{  0}, Score{ 10}, Score{ 15}, Score{ 15}, Score{ 10}, Score{  0}, Score{-30},
            Score{-40}, Score{-20}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{-20}, Score{-40},
            Score{-50}, Score{-40}, Score{-30}, Score{-30}, Score{-30}, Score{-30}, Score{-40}, Score{-50}
        },
        // Bishop
        {
            Score{-20}, Score{-10}, Score{-10}, Score{-10}, Score{-10}, Score{-10}, Score{-10}, Score{-20},
            Score{-10}, Score{  5}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{  5}, Score{-10},
            Score{-10}, Score{ 10}, Score{ 10}, Score{ 10}, Score{ 10}, Score{ 10}, Score{ 10}, Score{-10},
            Score{-10}, Score{  0}, Score{ 10}, Score{ 10}, Score{ 10}, Score{ 10}, Score{  0}, Score{-10},
            Score{-10}, Score{  5}, Score{  5}, Score{ 10}, Score{ 10}, Score{  5}, Score{  5}, Score{-10},
            Score{-10}, Score{  0}, Score{  5}, Score{ 10}, Score{ 10}, Score{  5}, Score{  0}, Score{-10},
            Score{-10}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{-10},
            Score{-20}, Score{-10}, Score{-10}, Score{-10}, Score{-10}, Score{-10}, Score{-10}, Score{-20}
        },
        // Rook
        {
            Score{  0}, Score{  0}, Score{  0}, Score{  5}, Score{  5}, Score{  0}, Score{ 0}, Score{  0},
            Score{ -5}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{ 0}, Score{ -5},
            Score{ -5}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{ 0}, Score{ -5},
            Score{ -5}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{ 0}, Score{ -5},
            Score{ -5}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{ 0}, Score{ -5},
            Score{ -5}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{ 0}, Score{ -5},
            Score{  5}, Score{ 10}, Score{ 10}, Score{ 10}, Score{ 10}, Score{ 10}, Score{10}, Score{  5},
            Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{ 0}, Score{  0}
        }, 
        // Queen
        {
            Score{-20}, Score{-10}, Score{-10}, Score{ -5}, Score{ -5}, Score{-10}, Score{-10}, Score{-20},
            Score{-10}, Score{  0}, Score{  5}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{-10},
            Score{-10}, Score{  5}, Score{  5}, Score{  5}, Score{  5}, Score{  5}, Score{  0}, Score{-10},
            Score{  0}, Score{  0}, Score{  5}, Score{  5}, Score{  5}, Score{  5}, Score{  0}, Score{ -5},
            Score{ -5}, Score{  0}, Score{  5}, Score{  5}, Score{  5}, Score{  5}, Score{  0}, Score{ -5},
            Score{-10}, Score{  0}, Score{  5}, Score{  5}, Score{  5}, Score{  5}, Score{  0}, Score{-10},
            Score{-10}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{-10},
            Score{-20}, Score{-10}, Score{-10}, Score{ -5}, Score{ -5}, Score{-10}, Score{-10}, Score{-20}
        }, 
        // King (middle game)
        {
            Score{ 20}, Score{ 30}, Score{ 10}, Score{  0}, Score{  0}, Score{ 10}, Score{ 30}, Score{ 20},
            Score{ 20}, Score{ 20}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{ 20}, Score{ 20},
            Score{-10}, Score{-20}, Score{-20}, Score{-20}, Score{-20}, Score{-20}, Score{-20}, Score{-10},
            Score{-20}, Score{-30}, Score{-30}, Score{-40}, Score{-40}, Score{-30}, Score{-30}, Score{-20},
            Score{-30}, Score{-40}, Score{-40}, Score{-50}, Score{-50}, Score{-40}, Score{-40}, Score{-30},
            Score{-30}, Score{-40}, Score{-40}, Score{-50}, Score{-50}, Score{-40}, Score{-40}, Score{-30},
            Score{-30}, Score{-40}, Score{-40}, Score{-50}, Score{-50}, Score{-40}, Score{-40}, Score{-30},
            Score{-30}, Score{-40}, Score{-40}, Score{-50}, Score{-50}, Score{-40}, Score{-40}, Score{-30}
        },
        // King (end game)
        {
            Score{-50}, Score{-30}, Score{-30}, Score{-30}, Score{-30}, Score{-30}, Score{-30}, Score{-50},
            Score{-30}, Score{-30}, Score{  0}, Score{  0}, Score{  0}, Score{  0}, Score{-30}, Score{-30},
            Score{-30}, Score{-10}, Score{ 20}, Score{ 30}, Score{ 30}, Score{ 20}, Score{-10}, Score{-30},
            Score{-30}, Score{-10}, Score{ 30}, Score{ 40}, Score{ 40}, Score{ 30}, Score{-10}, Score{-30},
            Score{-30}, Score{-10}, Score{ 30}, Score{ 40}, Score{ 40}, Score{ 30}, Score{-10}, Score{-30},
            Score{-30}, Score{-10}, Score{ 20}, Score{ 30}, Score{ 30}, Score{ 20}, Score{-10}, Score{-30},
            Score{-30}, Score{-20}, Score{-10}, Score{  0}, Score{  0}, Score{-10}, Score{-20}, Score{-30},
            Score{-50}, Score{-40}, Score{-30}, Score{-20}, Score{-20}, Score{-30}, Score{-40}, Score{-50}
        }
        // clang-format on
    };

    Score pawn_promotion_score{100}; ///< Bonus for a promoting pawn.

    /**
     * \brief Get the value for a piece of a given type.
     *
     * \param piece_type Type of the piece.
     * \return Value for a piece of the given type.
     */
    auto piece_value(chess_core::PieceType piece_type) const -> Score { return piece_values[get_index(piece_type)]; }

    /**
     * \brief Gives the value for a piece on a square.
     *
     * Values for the king should be used during middle game. For the end game,
     * use king_on_square_value(), which can interpolate between middle game and
     * end game values.
     * \param piece The piece.
     * \param square The square.
     * \return The value for the given piece on the given square.
     */
    auto piece_on_square_value(chess_core::Piece piece, const chess_core::Square &square) const -> Score {
        const auto lookup_square = (piece.color() == chess_core::Color::White) ? square : square.mirrored();
        return piece_square_tables[get_index(piece.type())].value(lookup_square);
    }

    /**
     * \brief Evaluate the kings position on the board.
     *
     * Returns the value for the kings position on the board. Can interpolate
     * between tables for middle game and end game. The tables are weighted with
     * the given factor. A factor of 1.0 means middle game, a factor of 0.0
     * means end game. Factors in between mix the values for the tables
     * accordingly.
     * \param square The square where the king is.
     * \param color Color of the king.
     * \param middlegame_factor Factor for weighting the middle game table vs. end game table (1.0 -> middle game; 0.0 -> end game).
     * \return Value for the king on the given square.
     */
    auto king_on_square_value(const chess_core::Square &square, chess_core::Color color, float middlegame_factor = 1.0F) const -> Score {
        const auto lookup_square = color == chess_core::Color::White ? square : square.mirrored();
        const auto middlegame_value = piece_square_tables[6].value(lookup_square).value;
        const auto endgame_value = piece_square_tables[7].value(lookup_square).value;
        return Score{static_cast<Score::value_type>(middlegame_value * middlegame_factor + endgame_value * (1.0F - middlegame_factor))};
    }

    auto empty_board_value() const -> Score { return Score{0}; }

    static_assert(get_index(chess_core::PieceType::Pawn) == 0);
    static_assert(get_index(chess_core::PieceType::Knight) == 1);
    static_assert(get_index(chess_core::PieceType::Bishop) == 2);
    static_assert(get_index(chess_core::PieceType::Rook) == 3);
    static_assert(get_index(chess_core::PieceType::Queen) == 4);
    static_assert(get_index(chess_core::PieceType::King) == 5);
};

/**
 * \brief Configuration for the chess engine.
 *
 * Holds values for the different parameters of the chess engine.
 */
struct Config {
    MinimaxConfig minimax_config;     ///< Configuration of the search algorithm.
    SearchConfig search_config;       ///< Configuration of the search strategy.
    EvaluatorConfig evaluator_config; ///< Configuration of the evaluation function.

    /**
     * \brief Read the configuration from a file.
     *
     * Reads the configuration parameters for the chess engine from the given file.
     * \param filename Path to the config file.
     * \return The config parameters.
     */
    static auto from_file(const std::filesystem::path &filename) -> Config;
};

} // namespace chess_engine

#endif
