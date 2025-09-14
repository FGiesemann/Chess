add_executable(perft
    main.cpp
    perft.cpp
)

target_link_libraries(perft
    PRIVATE
    chesscore::chesscore
    chesscore::chesscore_io
)
