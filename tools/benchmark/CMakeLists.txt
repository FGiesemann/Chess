add_executable(benchmark
    benchmark.cpp
    main.cpp
    perft_benchmark.cpp
)

target_link_libraries(benchmark
    PRIVATE
    chesscore::chesscore
)

add_optimization_settings(benchmark)
add_compiler_warnings(benchmark)
