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

enable_utf8_output(fen_display)

add_optimization_settings(fen_display)

install(TARGETS fen_display
    EXPORT ChessTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT tools
)
