cmake_minimum_required(VERSION 3.16)

project(ChessTools VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(chesscore CONFIG REQUIRED COMPONENTS chesscore chesscore_io)
find_package(chessgame REQUIRED)
find_package(chessengine REQUIRED)

include(FetchContent)
FetchContent_Declare(
    ChessBuild
    GIT_REPOSITORY https://github.com/FGiesemann/ChessBuild.git
    GIT_TAG        v0.8.5
)
FetchContent_MakeAvailable(ChessBuild)
message(STATUS "CHESS BUILD MODULE DIR: ${CHESS_BUILD_MODULE_DIR}")
list(APPEND CMAKE_MODULE_PATH ${CHESS_BUILD_MODULE_DIR})
include(CompilerWarnings)
include(CompilerSettings)

add_subdirectory(benchmark)
add_subdirectory(bitmap_tool)
add_subdirectory(chess_game)
add_subdirectory(fen_display)
add_subdirectory(legal_moves_in_position_tool)
add_subdirectory(perft)
add_subdirectory(pgn_parser)
add_subdirectory(table_generator)
add_subdirectory(test_game_filter)
