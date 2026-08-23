add_executable(mate_in_x_test
    src/main.cpp
    src/mate_in_x.cpp
)
target_link_libraries(mate_in_x_test
    PRIVATE
    Chess::engine
    Chess::game
)
enable_exception_handling(mate_in_x_test)
add_optimization_settings(mate_in_x_test)

install(TARGETS mate_in_x_test
    EXPORT ChessTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT chess_engine
)

add_executable(lichess_converter
    src/lichess_converter.cpp
    src/multi_solution_finder.cpp
)
target_link_libraries(lichess_converter
    PRIVATE
    Chess::core
    Chess::game
    Chess::uci
)
target_compile_features(lichess_converter PRIVATE cxx_std_23)
enable_exception_handling(lichess_converter)
add_optimization_settings(lichess_converter)

install(TARGETS lichess_converter
    EXPORT ChessTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT chess_engine
)
