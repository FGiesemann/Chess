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
