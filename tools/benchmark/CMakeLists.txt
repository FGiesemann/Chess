add_executable(benchmark
    main.cpp
    benchmark.cpp
)

target_link_libraries(benchmark
    PRIVATE
    chesscore::chesscore
)

add_optimization_settings(benchmark)
add_compiler_warnings(benchmark)
