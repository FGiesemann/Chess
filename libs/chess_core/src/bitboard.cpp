/* ************************************************************************** *
 * Chess Core                                                                 *
 * Data structures and algorithms for chess                                   *
 * ************************************************************************** */

#include "chesscore/bitboard.h"
#include "chesscore/bitboard_tables.h"

namespace chesscore {

namespace {

auto step_pawns(const Bitmap &pawn, Color side_to_move) -> Bitmap {
    return side_to_move == Color::White ? pawn << File::max_file : pawn >> File::max_file;
}

auto shift_left(const Bitmap &bitmap) -> Bitmap {
    // we remove pieces from the a-file, so they don't "wrap around" when shifting
    return (bitmap & ~bitmaps::file_table[File::min_file]) >> 1;
}

auto shift_right(const Bitmap &bitmap) -> Bitmap {
    // we remove pieces from the h-file, so they don't "wrap around" when shifting
    return (bitmap & ~bitmaps::file_table[File::max_file]) << 1;
}

auto generate_pawn_move(
    const Square &source, const Square &target, std::optional<Piece> captured, bool en_passant, std::optional<Piece> promoted, const PositionState &state, MoveList &moves
) -> void {
    moves.emplace_back(
        Move{
            .from = source,
            .to = target,
            .piece = Piece{.type = PieceType::Pawn, .color = state.side_to_move},
            .captured = captured,
            .capturing_en_passant = en_passant,
            .promoted = promoted,
            .castling_rights_before = state.castling_rights,
            .halfmove_clock_before = state.halfmove_clock,
            .en_passant_target_before = state.en_passant_target
        }
    );
}

auto generate_pawn_moves(const Square &source, const Square &target, std::optional<Piece> captured, bool en_passant, const PositionState &state, MoveList &moves) -> void {
    if (target.rank().rank == Rank::min_rank || target.rank().rank == Rank::max_rank) {
        const auto color = other_color(state.side_to_move);
        for (const auto &type : all_promotion_piece_types) {
            generate_pawn_move(source, target, captured, en_passant, Piece{.type = type, .color = color}, state, moves);
        }
    } else {
        generate_pawn_move(source, target, captured, en_passant, std::nullopt, state, moves);
    }
}

} // namespace

Bitboard::Bitboard(const FenString &fen) {
    const auto &placements{fen.piece_placement()};
    for (int rank{Rank::min_rank}; rank <= Rank::max_rank; ++rank) {
        for (int file{File::min_file}; file <= File::max_file; ++file) {
            const auto square = Square{rank, file};
            const auto piece{placements[square.index()]};
            if (piece) {
                set_piece(piece.value(), square);
            }
        }
    }
}

auto Bitboard::empty() const -> bool {
    return m_all_pieces.empty();
}

auto Bitboard::has_piece(const PieceType &piece_type) const -> bool {
    return !(bitmap({.type = piece_type, .color = Color::White}) | bitmap({.type = piece_type, .color = Color::Black})).empty();
}

auto Bitboard::has_piece(const Piece &piece) const -> bool {
    return !bitmap(piece).empty();
}

auto Bitboard::has_piece(const Color &color) const -> bool {
    return !bitmap(color).empty();
}

auto Bitboard::has_piece(const Square &square) const -> bool {
    return m_all_pieces.get(square);
}

auto Bitboard::set_piece(const Piece &piece, const Square &square) -> void {
    clear_square(square);
    bitmap(piece).set(square);
    m_all_pieces.set(square);
    bitmap(piece.color).set(square);
}

auto Bitboard::get_piece(const Square &square) const -> std::optional<Piece> {
    if (!m_all_pieces.get(square)) {
        return {};
    }
    const auto [min_index, max_index] =
        (m_white_pieces.get(square)) ? std::make_pair(min_white_piece_index, max_white_piece_index) : std::make_pair(min_black_piece_index, max_black_piece_index);
    for (size_t i = min_index; i <= max_index; ++i) {
        if (m_bitmaps.at(i).get(square)) {
            return Piece{.type = static_cast<PieceType>(i - min_index), .color = (i <= max_white_piece_index) ? Color::White : Color::Black};
        }
    }
    return {};
}

auto Bitboard::clear_square(const Square &square) -> void {
    const auto remove_from_square = ~Bitmap{square};

    for (auto &bitmap : m_bitmaps) {
        bitmap &= remove_from_square;
    }

    m_white_pieces &= remove_from_square;
    m_black_pieces &= remove_from_square;
    m_all_pieces &= remove_from_square;
}

auto Bitboard::make_move(const Move &move) -> void {
    clear_square(move.from);
    if (move.promoted) {
        set_piece(move.promoted.value(), move.to);
    } else {
        set_piece(move.piece, move.to);
    }
    if (move.is_castling()) {
        move_castling_rook(move);
    }
    if (move.capturing_en_passant) {
        clear_square(Square{move.to.file(), move.from.rank()});
    }
}

void Bitboard::move_castling_rook(const Move &move) {
    if (move.from.file().file < move.to.file().file) {
        // Kingside castling
        clear_square(Square{File{'H'}, move.to.rank()});                                                         // remove rook
        set_piece(Piece{.type = PieceType::Rook, .color = move.piece.color}, Square{File{'F'}, move.to.rank()}); // place rook on f-file
    } else {
        // Queenside castling
        clear_square(Square{File{'A'}, move.to.rank()});                                                         // remove rook
        set_piece(Piece{.type = PieceType::Rook, .color = move.piece.color}, Square{File{'D'}, move.to.rank()}); // place rook on d-file
    }
}

auto Bitboard::unmake_move(const Move &move) -> void {
    set_piece(move.piece, move.from);
    if (move.captured) {
        if (move.capturing_en_passant) {
            set_piece(move.captured.value(), Square{move.to.file(), move.from.rank()});
            clear_square(move.to);
        } else {
            set_piece(move.captured.value(), move.to);
        }
    } else {
        clear_square(move.to);
    }
    if (move.is_castling()) {
        reset_castling_rook(move);
    }
}

auto Bitboard::reset_castling_rook(const Move &move) -> void {
    if (move.from.file().file < move.to.file().file) {
        // Kingside castling
        clear_square(Square{File{'F'}, move.from.rank()});                                                         // remove rook
        set_piece(Piece{.type = PieceType::Rook, .color = move.piece.color}, Square{File{'H'}, move.from.rank()}); // place rook on h-file
    } else {
        // Queenside castling
        clear_square(Square{File{'D'}, move.from.rank()});                                                         // remove rook
        set_piece(Piece{.type = PieceType::Rook, .color = move.piece.color}, Square{File{'A'}, move.from.rank()}); // place rook on a-file
    }
}

auto Bitboard::all_legal_moves(const PositionState &state) const -> MoveList {
    MoveList moves{};
    all_knight_moves(moves, state);
    all_king_moves(moves, state);
    return moves;
}

auto Bitboard::all_stepping_moves(PieceType piece_type, MoveList &moves, const PositionState &state) const -> void {
    const auto piece = Piece{.type = piece_type, .color = state.side_to_move};
    Bitmap pieces{bitmap(piece)};

    Square pos{Square::A1};
    while (!pieces.empty()) {
        const auto shift = pieces.empty_squares_before();
        pos += shift;
        pieces >>= shift;

        auto targets = bitmaps::get_target_table(piece_type)[pos] & ~bitmap(state.side_to_move);
        extract_moves(targets, pos, piece, state, moves);

        pos += 1;
        pieces >>= 1;
    }
}

auto Bitboard::all_knight_moves(MoveList &moves, const PositionState &state) const -> void {
    all_stepping_moves(PieceType::Knight, moves, state);
}

auto Bitboard::all_king_moves(MoveList &moves, const PositionState &state) const -> void {
    all_stepping_moves(PieceType::King, moves, state);
}

auto Bitboard::all_sliding_moves(const Piece &moving_piece, const Square &start, MoveList &moves, const PositionState &state) const -> void {
    const auto ray_directions_for_piece = piece_ray_directions[moving_piece.type];
    for (auto direction : all_ray_directions) {
        if (ray_directions_for_piece & direction) {
            all_moves_along_ray(moving_piece, start, direction, moves, state);
        }
    }
}

auto Bitboard::all_moves_along_ray(const Piece &moving_piece, const Square &start, const RayDirection &direction, MoveList &moves, const PositionState &state) const -> void {
    auto targets = bitmaps::ray_target_table[direction][start];
    const auto blockers = targets & m_all_pieces;
    if (!blockers.empty()) {
        const auto blocker_square = Square::A1 + (is_negative_direction(direction) ? 63 - blockers.empty_squares_after() : blockers.empty_squares_before());
        targets ^= bitmaps::ray_target_table[direction][blocker_square];
    }
    targets &= ~bitmap(state.side_to_move);
    extract_moves(targets, start, moving_piece, state, moves);
}

auto Bitboard::remove_occupied_squares(const Bitmap &bitmap) const -> Bitmap {
    return bitmap & ~m_all_pieces;
}

auto Bitboard::all_pawn_moves([[maybe_unused]] MoveList &moves, const PositionState &state) const -> void {
    const auto pawns = bitmap(Piece{.type = PieceType::Pawn, .color = state.side_to_move});
    const auto pawns_advance1 = step_pawns(pawns, state.side_to_move);
    const auto pawns_step1 = remove_occupied_squares(pawns_advance1);
    extract_pawn_moves(pawns_step1, 1, state, moves);

    const auto double_step_mask = // pawns have already advanced one step, therefore we use the incremented/decremented ranks here
        state.side_to_move == Color::White ? bitmaps::rank_table[Rank{Rank::white_pawn_double_step_rank + 1}] : bitmaps::rank_table[Rank{Rank::black_pawn_double_step_rank - 1}];
    const auto pawns_double_candidates = pawns_step1 & double_step_mask;
    const auto pawns_advance2 = step_pawns(pawns_double_candidates, state.side_to_move);
    const auto pawns_step2 = remove_occupied_squares(pawns_advance2);
    extract_pawn_moves(pawns_step2, 2, state, moves);

    const auto captureable_pieces =
        state.en_passant_target.has_value() ? bitmap(other_color(state.side_to_move)) | Bitmap{state.en_passant_target.value()} : bitmap(other_color(state.side_to_move));
    const auto pawns_W = shift_left(pawns_advance1);
    const auto pawns_capture_W = pawns_W & captureable_pieces;
    extract_pawn_captures(pawns_capture_W, PawnCaptureDirection::West, state, moves);
    const auto pawns_E = shift_right(pawns_advance1);
    const auto pawns_capture_E = pawns_E & captureable_pieces;
    extract_pawn_captures(pawns_capture_E, PawnCaptureDirection::East, state, moves);
}

auto Bitboard::is_attacked([[maybe_unused]] const Square &square, [[maybe_unused]] Color attacker_color) const -> bool {
    // Check in "reverse":
    // - use "pawn_attack_table[color][square]" to find pawns that attack square
    // - use knight_moves[square] & m_knights[attacker_color] to find knights that attack square
    // - use ray checks from the square to detect sliding pieces that attack square
    return pawn_attacks(square, attacker_color) || knight_attacks(square, attacker_color) || sliding_piece_attacks(square, attacker_color);
}

auto Bitboard::would_be_attacked(const Square &square, Color attacker_color, [[maybe_unused]] const Move &move) const -> bool {
    // - Modify the necessary bitmasks used in is_attacked (or more precise sliding_piece_attacks)
    bool under_attack = is_attacked(square, attacker_color);
    // - Restore the bitmasks
    return under_attack;
}

auto Bitboard::pawn_attacks(const Square &square, Color pawn_color) const -> bool {
    const auto pawns = bitmap(Piece{.type = PieceType::Pawn, .color = pawn_color});
    const auto stepped_pawns = step_pawns(pawns, pawn_color);
    const auto attacked_squares = shift_left(stepped_pawns) | shift_right(stepped_pawns);
    return attacked_squares.get(square);
}

auto Bitboard::knight_attacks(const Square &square, Color knight_color) const -> bool {
    const auto knights = bitmap(Piece{.type = PieceType::Knight, .color = knight_color});
    const auto attackers = bitmaps::get_target_table(PieceType::Knight)[square] & knights;
    return !attackers.empty();
}

auto Bitboard::sliding_piece_attacks([[maybe_unused]] const Square &square, [[maybe_unused]] Color piece_color) const -> bool {
    // - use ray checks from the square to detect sliding pieces that attack square
    return false;
}

auto Bitboard::extract_moves(Bitmap targets, const Square &from, const Piece &piece, const PositionState &state, MoveList &moves) const -> void {
    Square target_square{Square::A1};
    while (!targets.empty()) {
        const auto shift = targets.empty_squares_before();
        target_square += shift;
        targets >>= shift;
        moves.emplace_back(
            Move{
                .from = from,
                .to = target_square,
                .piece = piece,
                .captured = get_piece(target_square),
                .capturing_en_passant = false,
                .promoted = {},
                .castling_rights_before = state.castling_rights,
                .halfmove_clock_before = state.halfmove_clock,
                .en_passant_target_before = state.en_passant_target
            }
        );
        target_square += 1;
        targets >>= 1;
    }
}

void Bitboard::extract_pawn_moves(Bitmap targets, int step_size, const PositionState &state, MoveList &moves) {
    Square target_square{Square::A1};
    while (!targets.empty()) {
        const auto shift = targets.empty_squares_before();
        target_square += shift;
        targets >>= shift + 1;
        const auto source_square = state.side_to_move == Color::White ? (target_square - File::max_file * step_size) : (target_square + File::max_file * step_size);
        generate_pawn_moves(source_square, target_square, std::nullopt, false, state, moves);
        target_square += 1;
    }
}

auto Bitboard::extract_pawn_captures(Bitmap targets, PawnCaptureDirection direction, const PositionState &state, MoveList &moves) const -> void {
    Square target_square{Square::A1};
    while (!targets.empty()) {
        const auto shift = targets.empty_squares_before();
        target_square += shift;
        targets >>= shift + 1;
        const auto source_square = Square{
            File{direction == PawnCaptureDirection::East ? target_square.file().file - 1 : target_square.file().file + 1},
            Rank{state.side_to_move == Color::White ? target_square.rank().rank - 1 : target_square.rank().rank + 1},
        };
        const auto captured = get_piece(target_square);
        generate_pawn_moves(
            source_square, target_square, captured.value_or(Piece{.type = PieceType::Pawn, .color = other_color(state.side_to_move)}), !captured.has_value(), state, moves
        );
        target_square += 1;
    }
}

} // namespace chesscore
