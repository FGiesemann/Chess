add_executable(pgn_parser
    pgn_parser.cpp
)

target_link_libraries(pgn_parser
    PRIVATE
    ChessGame::ChessGame
)

target_compile_options(pgn_parser PRIVATE
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
)
