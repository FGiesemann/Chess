cmake_minimum_required(VERSION 3.15)
project(chesscore CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/compiler_warnings.cmake)

add_library(${PROJECT_NAME}
    src/chesscore/bitboard.cpp
    src/chesscore/bitboard_tables.cpp
    src/chesscore/bitmap.cpp
    src/chesscore/board.cpp
    src/chesscore/chesscore.cpp
    src/chesscore/fen.cpp
    src/chesscore/move.cpp
    src/chesscore/piece.cpp
    src/chesscore/position.cpp
    src/chesscore/position_types.cpp
    src/chesscore/square.cpp
    src/chesscore/table.cpp
)
target_include_directories(chesscore PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
add_compiler_warnings(chesscore)

add_library(chesscore_io
        src/chesscore_io/bitboard_io.cpp
        src/chesscore_io/bitmap_io.cpp
        src/chesscore_io/piece_io.cpp
)
target_link_libraries(chesscore_io PRIVATE ${PROJECT_NAME})
target_compile_options(chesscore_io PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

    add_custom_target(doc ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating documentation with Doxygen"
        VERBATIM
    )
endif()

enable_testing()
find_package(Catch2 3 REQUIRED)
add_subdirectory(test)
