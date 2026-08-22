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

target_compile_options(magic_bitboard_generator PRIVATE
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
)

add_optimization_settings(magic_bitboard_generator)
