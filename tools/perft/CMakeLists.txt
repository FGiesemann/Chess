add_executable(perft
    main.cpp
    perft.cpp
    suite.cpp
)

target_link_libraries(perft
    PRIVATE
    chesscore::chesscore
    chesscore::chesscore_io
)
