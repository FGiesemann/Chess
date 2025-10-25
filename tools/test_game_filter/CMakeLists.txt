add_executable(test_game_filter
    main.cpp
)

target_link_libraries(test_game_filter
    PRIVATE
    chesscore::chesscore
    chesscore::chesscore_io
    ChessGame::ChessGame
)
