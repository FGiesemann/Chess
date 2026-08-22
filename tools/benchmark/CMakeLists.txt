add_executable(benchmark
    src/benchmark.cpp
    src/engine_benchmark.cpp
    src/main.cpp
    src/perft_benchmark.cpp
)

target_link_libraries(benchmark
    PRIVATE
    chesscore::chesscore
    ChessEngine::ChessEngineLib
)

add_optimization_settings(benchmark)
add_compiler_warnings(benchmark)
