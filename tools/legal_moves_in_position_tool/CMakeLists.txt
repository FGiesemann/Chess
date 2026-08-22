add_executable(all_legal_moves)

target_sources(all_legal_moves
    PRIVATE
        legal_moves_tool.cpp
)

target_link_libraries(all_legal_moves
    PRIVATE
    Chess::core
    Chess::core_io
)

target_compile_options(all_legal_moves PRIVATE
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
)

add_optimization_settings(all_legal_moves)
