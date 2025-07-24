cmake_minimum_required(VERSION 3.16)

project(ChessTools VERSION 1.0.0 LANGUAGES CXX)

find_package(chesscore CONFIG REQUIRED COMPONENTS chesscore chesscore_io)

add_subdirectory(bitmap_tool)
add_subdirectory(fen_display)
add_subdirectory(table_generator)
