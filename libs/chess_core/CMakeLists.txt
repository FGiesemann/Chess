cmake_minimum_required(VERSION 3.15)
project(chesscore CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()
find_package(Catch2 3 REQUIRED)
include(CTest)
add_subdirectory(test)

# find_program(CLANGTIDY_EXECUTABLE NAMES clang-tidy HINTS "C:/Program Files/LLVM/bin")
# set(CLANGTIDY_EXECUTABLE "${CLANGTIDY_EXECUTABLE}" CACHE FILEPATH "Path to the preferred clang-tidy executable")
# if(NOT EXISTS "${CLANGTIDY_EXECUTABLE}" OR "${CLANGTIDY_EXECUTABLE}" STREQUAL "")
#     message(FATAL_ERROR "CLANGTIDY_EXECUTABLE is not set or points to a non-existent file.")
# endif()
# set(CMAKE_CXX_CLANG_TIDY "${CLANGTIDY_EXECUTABLE}")

add_library(chesscore
    src/chesscore.cpp
    src/board.cpp
)

target_include_directories(chesscore PUBLIC include)

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
