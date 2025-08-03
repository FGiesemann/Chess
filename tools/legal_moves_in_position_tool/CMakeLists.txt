add_executable(all_legal_moves
    legal_moves_tool.cpp
)

target_link_libraries(all_legal_moves
    PRIVATE
    chesscore::chesscore
    chesscore::chesscore_io
)

target_compile_options(all_legal_moves PRIVATE
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
)
