add_executable(benchmark)

target_sources(benchmark
    PRIVATE
        src/benchmark.cpp
        src/engine_benchmark.cpp
        src/main.cpp
        src/perft_benchmark.cpp
)

target_link_libraries(benchmark
    PRIVATE
    Chess::core
    Chess::engine
)

add_optimization_settings(benchmark)
add_compiler_warnings(benchmark)
