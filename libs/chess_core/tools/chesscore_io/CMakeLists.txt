add_library(chesscore_io
    bitmap_io.cpp
)
target_include_directories(chesscore_io PUBLIC include)

target_link_libraries(chesscore_io PUBLIC chesscore)

target_compile_options(chesscore_io PUBLIC
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
)
