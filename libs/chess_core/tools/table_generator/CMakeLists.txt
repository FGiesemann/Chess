add_executable(table_generator
    main.cpp
    bigmap.cpp
    table_gen.cpp
    patterns.cpp
)

target_link_libraries(table_generator
    chesscore
    chesscore_io
)
