add_executable(magic_bitboard_generator)

target_sources(magic_bitboard_generator
    PRIVATE
        main.cpp
        magic_bitboard.cpp
        database.cpp
        arg_parse.cpp
        search_scenario.cpp
        check_scenario.cpp
        generate_scenario.cpp
)

target_link_libraries(magic_bitboard_generator
    PRIVATE
        Chess::core
)

enable_utf8_output(magic_bitboard_generator)

add_optimization_settings(magic_bitboard_generator)

install(TARGETS magic_bitboard_generator
    EXPORT ChessTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
