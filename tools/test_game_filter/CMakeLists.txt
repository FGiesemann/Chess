add_executable(test_game_filter)

target_sources(test_game_filter
    PRIVATE
        main.cpp
)

target_link_libraries(test_game_filter
    PRIVATE
        Chess::core
        Chess::core_io
        Chess::game
)

add_optimization_settings(test_game_filter)

install(TARGETS test_game_filter
    EXPORT ChessTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
