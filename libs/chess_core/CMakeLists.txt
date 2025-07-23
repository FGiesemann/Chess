cmake_minimum_required(VERSION 3.15)
project(chesscore CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/compiler_warnings.cmake)

add_library(chesscore
    src/bitboard.cpp
    src/bitboard_tables.cpp
    src/bitmap.cpp
    src/board.cpp
    src/chesscore.cpp
    src/fen.cpp
    src/move.cpp
    src/piece.cpp
    src/position.cpp
    src/position_types.cpp
    src/square.cpp
    src/table.cpp
)
target_include_directories(chesscore PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
add_compiler_warnings(chesscore)

enable_testing()
find_package(Catch2 3 REQUIRED)
add_subdirectory(test)

add_subdirectory(tools)

install(TARGETS chesscore)

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
