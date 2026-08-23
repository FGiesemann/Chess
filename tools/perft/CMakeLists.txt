add_executable(perft)

target_sources(perft
    PRIVATE
        main.cpp
        perft.cpp
        suite.cpp
)

target_link_libraries(perft
    PRIVATE
        Chess::core
        Chess::core_io
)

add_optimization_settings(perft)

install(TARGETS perft
    EXPORT ChessTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT tools
)
