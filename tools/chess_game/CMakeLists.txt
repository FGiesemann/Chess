add_executable(chess_game_tool)

target_sources(chess_game_tool
    PRIVATE
        commands.cpp
        main.cpp
        user_io.cpp
)

target_link_libraries(chess_game_tool
    PRIVATE
        Chess::core
        Chess::core_io
        Chess::game
)

enable_utf8_output(chess_game_tool)

add_optimization_settings(chess_game_tool)

install(TARGETS chess_game_tool
    EXPORT ChessTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT tools
)
