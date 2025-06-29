/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */
/** \file */

#ifndef CHESSCORE_POSITION_H
#define CHESSCORE_POSITION_H

#include "chesscore/board.h"
#include "chesscore/fen.h"
#include "chesscore/piece.h"
#include "chesscore/square.h"

namespace chesscore {

/**
 * \brief The current state of a chess game.
 *
 * Represents the state of a game of chess. It includes the current placement of
 * pieces on the board, the current turn, castling rights, the playerr to move,
 * and many other pieces of information that describe the state of the game.
 * \tparam BoardT The board representation that manages the pieces on the board.
 */
template<Board BoardT>
class Position {
public:
    /**
     * \brief Create a new Position.
     *
     * The position holds an empty board, no castling rights and white to play.
     */
    Position() = default;

    /**
     * \brief Create a Position from a FEN string.
     *
     * The position is set up according to the FEN string.
     * \param fen The FEN string.
     */
    explicit Position(const FenString &fen)
        : m_board{fen}, m_side_to_move{fen.side_to_move()}, m_fullmove_number{fen.fullmove_number()}, m_halfmove_clock{fen.halfmove_clock()},
          m_castling_rights{fen.castling_rights()}, m_en_passant_target{fen.en_passant_square()} {}

    /**
     * \brief Access the board representation.
     *
     * The board representation manages the pieces on the board.
     * \return The board representation.
     */
    auto board() const -> const BoardT & { return m_board; }

    /**
     * \brief Return the player to move next.
     *
     * \return The player to move next.
     */
    auto side_to_move() const -> Color { return m_side_to_move; }

    /**
     * \brief Return the fullmove number.
     *
     * The fullmove number is the number of the next move. In this context, a
     * move consists of the half-moves of White and then Black. The fullmove
     * number increases, after Black plays.
     * \return The fullmove number.
     */
    auto fullmove_number() const -> size_t { return m_fullmove_number; }

    /**
     * \brief The halfmove clock.
     *
     * \return The halfmove clock.
     */
    auto halfmove_clock() const -> size_t { return m_halfmove_clock; }

    /**
     * \brief The current castling rights.
     *
     * Returns the rights to castle for each player.
     * \return The castling rights
     */
    auto castling_rights() const -> CastlingRights { return m_castling_rights; }

    /**
     * \brief An optional en-passant target square.
     *
     * If the last move was a double-step of a pawn, the overstepped square can
     * be the target of capturing en-passant in the following move. In that
     * case, this square is returned. If no capture by en-passant is possible,
     * this is an empty optional.
     * \return The possible en-passant target square.
     */
    auto en_passant_target() const -> std::optional<Square> { return m_en_passant_target; }

    /**
     * \brief Perform a move.
     *
     * Applies the given move in the current position. The move is assumed to be
     * valid in the current position. No validity cheks are performed!
     * \param move The move to apply.
     */
    auto make_move(const Move &move) -> void;

    /**
     * \brief Undo a move.
     *
     * Reverts the position back to the state before the move was made. This
     * only works, if the move was the last move made in this position. No
     * validity checks are performed!
     * \param move The move to undo.
     */
    auto unmake_move(const Move &move) -> void;
private:
    BoardT m_board{};                                         ///< Current placement of pieces on the board.
    Color m_side_to_move{Color::White};                       ///< The player who moves next.
    size_t m_fullmove_number{1};                              ///< Number of the next move.
    size_t m_halfmove_clock{0};                               ///< Half-move clock for the fifty-move rule.
    CastlingRights m_castling_rights{CastlingRights::none()}; ///< Castling rights.
    std::optional<Square> m_en_passant_target{};              ///< A possible en passant target square.

    auto updateCastlingRights(const Move &move) -> void;
    auto updateFullmoveNumber() -> void;
    auto updateHalfmoveClock(const Move &move) -> void;
    auto updateEnPassant(const Move &move) -> void;
    auto resetFullmoveNumber(const Move &move) -> void;
    auto resetHalfmoveClock(const Move &move) -> void;
    auto resetEnPassant(const Move &move) -> void;
    auto resetCastlingRights(const Move &move) -> void;
};

template<Board BoardT>
auto Position<BoardT>::make_move(const Move &move) -> void {
    m_board.make_move(move);
    updateFullmoveNumber();
    updateHalfmoveClock(move);
    updateEnPassant(move);
    updateCastlingRights(move);
    m_side_to_move = other_color(m_side_to_move);
}

template<Board BoardT>
void Position<BoardT>::updateFullmoveNumber() {
    if (m_side_to_move == Color::Black) {
        m_fullmove_number++;
    }
}

template<Board BoardT>
void Position<BoardT>::updateHalfmoveClock(const Move &move) {
    if (move.is_capture() || move.piece.type == PieceType::Pawn) {
        m_halfmove_clock = 0;
    } else {
        m_halfmove_clock++;
    }
}

template<Board BoardT>
void Position<BoardT>::updateEnPassant(const Move &move) {
    if (move.piece.type == PieceType::Pawn && move.is_double_step()) {
        if (move.from.rank().rank > move.to.rank().rank) {
            m_en_passant_target = Square{File{move.from.file().file}, Rank{move.from.rank().rank - 1}};
        } else {
            m_en_passant_target = Square{File{move.from.file().file}, Rank{move.from.rank().rank + 1}};
        }
    } else {
        m_en_passant_target.reset();
    }
}

template<Board BoardT>
void Position<BoardT>::updateCastlingRights(const Move &move) {
    if (move.piece == Piece::WhiteKing) {
        m_castling_rights['K'] = false;
        m_castling_rights['Q'] = false;
    } else if (move.piece == Piece::WhiteRook) {
        if (move.from == Square::H1) {
            m_castling_rights['K'] = false;
        } else if (move.from == Square::A1) {
            m_castling_rights['Q'] = false;
        }
    } else if (move.piece == Piece::BlackKing) {
        m_castling_rights['k'] = false;
        m_castling_rights['q'] = false;
    } else if (move.piece == Piece::BlackRook) {
        if (move.from == Square::H8) {
            m_castling_rights['k'] = false;
        } else if (move.from == Square::A8) {
            m_castling_rights['q'] = false;
        }
    } else if (move.is_capture() && move.to == Square::A1) {
        m_castling_rights['Q'] = false;
    } else if (move.is_capture() && move.to == Square::H1) {
        m_castling_rights['K'] = false;
    } else if (move.is_capture() && move.to == Square::A8) {
        m_castling_rights['q'] = false;
    } else if (move.is_capture() && move.to == Square::H8) {
        m_castling_rights['k'] = false;
    }
}

template<Board BoardT>
auto Position<BoardT>::unmake_move(const Move &move) -> void {
    m_board.unmake_move(move);
    resetFullmoveNumber(move);
    resetHalfmoveClock(move);
    resetEnPassant(move);
    resetCastlingRights(move);
    m_side_to_move = other_color(m_side_to_move);
}

template<Board BoardT>
auto Position<BoardT>::resetFullmoveNumber(const Move &move) -> void {
    if (m_side_to_move == Color::White) {
        m_fullmove_number--;
    }
}

template<Board BoardT>
auto Position<BoardT>::resetHalfmoveClock(const Move &move) -> void {
    m_halfmove_clock = move.halfmove_clock_before;
}

template<Board BoardT>
auto Position<BoardT>::resetEnPassant(const Move &move) -> void {
    if (move.capturing_en_passant) {
        m_en_passant_target = move.from;
    } else {
        m_en_passant_target.reset();
    }
}

template<Board BoardT>
auto Position<BoardT>::resetCastlingRights(const Move &move) -> void {
    m_castling_rights = move.castling_rights_before;
}

} // namespace chesscore

#endif
