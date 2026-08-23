add_executable(chess_game_tests
    src/pgn_lexer_test.cpp
    src/pgn_parser_test.cpp
    src/pgn_writer_test.cpp
    src/san_generator_test.cpp
    src/san_move_matcher_test.cpp
    src/san_parser_test.cpp
)
add_compiler_warnings(chess_game_tests)
enable_exception_handling(chess_game_tests)
target_link_libraries(chess_game_tests
  PRIVATE 
  Chess::game Chess::core_io
  Catch2::Catch2WithMain
)
add_optimization_settings(chess_game_tests)

catch_discover_tests(chess_game_tests)
