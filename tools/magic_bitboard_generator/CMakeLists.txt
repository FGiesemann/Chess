add_executable(magic_bitboard_generator
    main.cpp
    magic_bitboard.cpp
    database.cpp
)

target_link_libraries(magic_bitboard_generator
    PRIVATE
    chesscore::chesscore
)

target_compile_options(magic_bitboard_generator PRIVATE
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
)

add_optimization_settings(magic_bitboard_generator)
