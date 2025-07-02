add_executable(bitmap_tool
    bitmap_tool.cpp
)

target_link_libraries(bitmap_tool
    PRIVATE
    chesscore
)

target_compile_options(bitmap_tool PRIVATE
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
)
