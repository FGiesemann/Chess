add_executable(chessengine_tests
  src/depth_test.cpp
  src/evaluation_test.cpp
  src/score_test.cpp
  src/uci_engine_construct_position_test.cpp
  src/uci_engine_position_cb_test.cpp
)
add_compiler_warnings(chessengine_tests)
add_optimization_settings(chessengine_tests)
enable_exception_handling(chessengine_tests)
target_link_libraries(chessengine_tests
  PRIVATE 
  Chess::engine
  Catch2::Catch2WithMain
)

catch_discover_tests(chessengine_tests)
