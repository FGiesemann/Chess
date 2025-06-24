add_executable(chesscore_tests
    data/coordinate_test.cpp
    data/fen_test.cpp
    
    bitboard/bitmap_test.cpp
    bitboard/bitboard_test.cpp
)

target_link_libraries(chesscore_tests PRIVATE chesscore)
target_link_libraries(chesscore_tests PRIVATE Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(chesscore_tests)
