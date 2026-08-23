add_executable(chess_engine_tests
  src/depth_test.cpp
  src/evaluation_test.cpp
  src/score_test.cpp
  src/uci_engine_construct_position_test.cpp
  src/uci_engine_position_cb_test.cpp
)
add_compiler_warnings(chess_engine_tests)
add_optimization_settings(chess_engine_tests)
enable_exception_handling(chess_engine_tests)
target_link_libraries(chess_engine_tests
  PRIVATE 
  Chess::engine
  Catch2::Catch2WithMain
)

catch_discover_tests(chess_engine_tests)
