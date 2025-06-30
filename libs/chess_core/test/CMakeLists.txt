add_executable(chesscore_tests
    data/coordinate_test.cpp
    data/fen_test.cpp
    data/piece_test.cpp
    
    bitboard/bitmap_test.cpp
    bitboard/bitboard_test.cpp
    bitboard/make_move_test.cpp
    bitboard/unmake_move_test.cpp
    bitboard/move_generation_test.cpp

    position/position_test.cpp
    position/make_move_test.cpp
    position/unmake_move_test.cpp
)

target_link_libraries(chesscore_tests PRIVATE chesscore)
target_link_libraries(chesscore_tests PRIVATE Catch2::Catch2WithMain)

# Disable warnings for braced scalar initialization in Clang (erroneous warning for designated initializers)
target_compile_options(chesscore_tests PRIVATE
    $<$<CXX_COMPILER_ID:Clang>:-Wno-braced-scalar-init>
)

include(Catch)
catch_discover_tests(chesscore_tests)
