add_executable(chess_game
    main.cpp
    user_io.cpp
)

target_link_libraries(chess_game
    PRIVATE
    chesscore::chesscore
    chesscore::chesscore_io
    ChessGame::ChessGame
)

target_compile_options(chess_game PRIVATE
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
)
