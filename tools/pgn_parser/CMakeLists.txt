add_executable(pgn_parser)

target_sources(pgn_parser
    PRIVATE
        pgn_parser.cpp
)

target_link_libraries(pgn_parser
    PRIVATE
    Chess::game
)

enable_utf8_output(pgn_parser)

add_optimization_settings(pgn_parser)

install(TARGETS pgn_parser
    EXPORT ChessTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT tools
)
