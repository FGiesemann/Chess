cmake_minimum_required(VERSION 3.15)
project(chesscore CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

function(add_compiler_warnings TARGET_NAME)
    target_compile_options(${TARGET_NAME} PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wall;-Wextra;-Wpedantic;-Wshadow;-Wformat=2;-Wconversion;-Wnon-virtual-dtor;-Wold-style-cast;-Wcast-align;-Wunused;-Wunreachable-code;-Wnull-dereference;-Wdouble-promotion;-Werror=return-type>
        $<$<CXX_COMPILER_ID:Clang>:-Wall;-Wextra;-Wpedantic;-Wshadow;-Wno-c++98-compat;-Wno-c++98-compat-pedantic;-Wno-pre-c++20-compat-pedantic;-Wformat=2;-Wconversion;-Wnon-virtual-dtor;-Wold-style-cast;-Wcast-align;-Wunused;-Wunreachable-code;-Wnull-dereference;-Wdouble-promotion;-Werror=return-type;-Werror=implicit-function-declaration;-Werror=incompatible-pointer-types>
        $<$<CXX_COMPILER_ID:MSVC>:/W4;/WX>
        $<$<CXX_COMPILER_ID:MSVC>:/permissive->
    )

    # Disable warnings for braced scalar initialization in Clang (erroneous warning for designated initializers)
    target_compile_options(${TARGET_NAME} PRIVATE
        $<$<CXX_COMPILER_ID:Clang>:-Wno-braced-scalar-init>
    )

    target_compile_options(${TARGET_NAME} PRIVATE
        $<$<AND:$<CXX_COMPILER_ID:GNU>,$<NOT:$<BOOL:CMAKE_DEBUG_POSTFIX>>>:-Werror>
        $<$<AND:$<CXX_COMPILER_ID:Clang>,$<NOT:$<BOOL:CMAKE_DEBUG_POSTFIX>>>:-Werror>
    )

    target_compile_options(${TARGET_NAME} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>
    )
endfunction()

add_library(chesscore
    src/bitboard.cpp
    src/board.cpp
    src/chesscore.cpp
    src/fen.cpp
    src/move.cpp
    src/piece.cpp
    src/position.cpp
    src/position_types.cpp
    src/square.cpp
)
target_include_directories(chesscore PUBLIC include)
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
