cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)

find_package(chesscore CONFIG REQUIRED COMPONENTS chesscore chesscore_io)

add_executable(test_without_io src/example.cpp)
target_link_libraries(test_without_io chesscore::chesscore)

add_executable(test_with_io src/example_io.cpp)
target_link_libraries(test_with_io 
    chesscore::chesscore
    chesscore::chesscore_io
)
