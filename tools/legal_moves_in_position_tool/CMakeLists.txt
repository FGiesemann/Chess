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

enable_utf8_output(all_legal_moves)

add_optimization_settings(all_legal_moves)

install(TARGETS all_legal_moves
    EXPORT ChessTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
