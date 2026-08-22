add_executable(table_generator
    main.cpp
    bigmap.cpp
    table_gen.cpp
    patterns.cpp
)

target_link_libraries(table_generator
    chesscore::chesscore
    chesscore::chesscore_io
)

add_optimization_settings(table_generator)
