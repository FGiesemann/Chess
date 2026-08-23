add_executable(chess_core_tests
    data/coordinate_test.cpp
    data/epd_test.cpp
    data/fen_test.cpp
    data/move_test.cpp
    data/piece_test.cpp
    data/zobrist_test.cpp
    
    bitboard/bitmap_test.cpp
    bitboard/bitboard_test.cpp
    bitboard/capture_move_generation_test.cpp
    bitboard/make_move_test.cpp
    bitboard/unmake_move_test.cpp
    bitboard/move_generation_test.cpp
    bitboard/attack_test.cpp

    position/hash_test.cpp
    position/make_move_test.cpp
    position/move_generation_test.cpp
    position/perft_test.cpp
    position/position_test.cpp
    position/unmake_move_test.cpp

    magic/block_mask_test.cpp
    magic/blocker_iteration_test.cpp
    magic/attack_map_test.cpp
)

add_compiler_warnings(chess_core_tests)
add_optimization_settings(chess_core_tests)

enable_exception_handling(chess_core_tests)

target_link_libraries(chess_core_tests PRIVATE Chess::core Chess::core_io Catch2::Catch2WithMain)

catch_discover_tests(chess_core_tests)
