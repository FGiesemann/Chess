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

target_compile_options(chess_game_tool PRIVATE
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
)

add_optimization_settings(chess_game_tool)
