add_executable(chesscore_tests
    coordinate_test.cpp
)

target_link_libraries(chesscore_tests PRIVATE chesscore)
target_link_libraries(chesscore_tests PRIVATE Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(chesscore_tests)
