add_executable(pgn_parser)

target_sources(pgn_parser
    PRIVATE
        pgn_parser.cpp
)

target_link_libraries(pgn_parser
    PRIVATE
    Chess::game
)

target_compile_options(pgn_parser PRIVATE
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
)

add_optimization_settings(pgn_parser)
