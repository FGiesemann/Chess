add_executable(fen_display
    fen_display.cpp
)

target_link_libraries(fen_display
    PRIVATE
    chesscore
    chesscore_io
)

target_compile_options(fen_display PRIVATE
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
)
