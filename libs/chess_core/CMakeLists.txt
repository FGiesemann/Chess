cmake_minimum_required(VERSION 3.15)
project(chesscore CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_CLANG_TIDY "Enable Clang-Tidy code analysis during build" OFF)

add_library(chesscore
    src/board.cpp
    src/chesscore.cpp
    src/fen.cpp
    src/piece.cpp
    src/position.cpp
    src/square.cpp
)
target_include_directories(chesscore PUBLIC include)

enable_testing()
find_package(Catch2 3 REQUIRED)
add_subdirectory(test)


find_program(CLANGTIDY_EXECUTABLE NAMES clang-tidy HINTS "C:/Program Files/LLVM/bin")
set(CLANGTIDY_EXECUTABLE "${CLANGTIDY_EXECUTABLE}" CACHE FILEPATH "Path to the preferred clang-tidy executable")
if(NOT CLANGTIDY_EXECUTABLE)
    message(WARNING "clang-tidy executable not found. Clang-Tidy analysis will be disabled.")
    set(ENABLE_CLANG_TIDY OFF)
endif()    
if(ENABLE_CLANG_TIDY AND CLANGTIDY_EXECUTABLE)
    message(STATUS "Clang-Tidy analysis enabled.")
    set(CLANG_TIDY_ARGS "")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CLANG_TIDY_ARGS "--extra-arg=-Wno-unknown-warning-option")
        list(APPEND CLANG_TIDY_ARGS "--extra-arg=/EHsc")
    endif()    
    set(CMAKE_CXX_CLANG_TIDY "${CLANGTIDY_EXECUTABLE}" "${CLANG_TIDY_ARGS}")
else()    
    message(STATUS "Clang-Tidy analysis disabled.")
    set(CMAKE_CXX_CLANG_TIDY "")
endif()    

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
