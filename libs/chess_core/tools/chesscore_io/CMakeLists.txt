add_library(chesscore_io
    bitboard_io.cpp
    bitmap_io.cpp
    piece_io.cpp
)
target_include_directories(chesscore_io PUBLIC include)

target_link_libraries(chesscore_io PUBLIC chesscore)

target_compile_options(chesscore_io PUBLIC
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
)
