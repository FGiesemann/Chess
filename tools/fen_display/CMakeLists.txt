add_executable(fen_display)

target_sources(fen_display
    PRIVATE
        fen_display.cpp
)

target_link_libraries(fen_display
    PRIVATE
    Chess::core
    Chess::core_io
)

target_compile_options(fen_display PRIVATE
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
)

add_optimization_settings(fen_display)
