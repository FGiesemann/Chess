add_executable(chesscore_tests
    data/coordinate_test.cpp
    data/fen_test.cpp
    data/move_test.cpp
    data/piece_test.cpp
    
    bitboard/bitmap_test.cpp
    bitboard/bitboard_test.cpp
    bitboard/make_move_test.cpp
    bitboard/unmake_move_test.cpp
    bitboard/move_generation_test.cpp
    bitboard/attack_test.cpp

    position/position_test.cpp
    position/make_move_test.cpp
    position/unmake_move_test.cpp
    position/move_generation_test.cpp
)
add_compiler_warnings(chesscore_tests)

target_link_libraries(chesscore_tests PRIVATE chesscore Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(chesscore_tests)
