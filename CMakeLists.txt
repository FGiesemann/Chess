cmake_minimum_required(VERSION 3.23)
project(Chess VERSION 1.0.0 LANGUAGES CXX)

include(CTest)
include(cmake/CompilerSettings.cmake)
include(cmake/CompilerWarnings.cmake)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_DOCUMENTATION "Build Doxygen documentation" OFF)

option(WITH_CHESS_GUI "Build chess gui" OFF)

if (BUILD_TESTING)
    find_package(Catch2 3 REQUIRED)
    include(Catch)
endif()

if (BUILD_DOCUMENTATION)
    find_package(Doxygen REQUIRED)

    add_custom_target(docs
        COMMENT "Generate documentation for all components"
    )
endif()

add_subdirectory(libs/chess_core)
add_subdirectory(libs/chess_game)
add_subdirectory(libs/chess_uci)
add_subdirectory(libs/chess_engine)

if (WITH_CHESS_GUI)
    add_subdirectory(libs/chess_gui)
endif()

add_subdirectory(tools/benchmark)
add_subdirectory(tools/bitmap_tool)
add_subdirectory(tools/chess_game)
add_subdirectory(tools/fen_display)
add_subdirectory(tools/legal_moves_in_position_tool)
add_subdirectory(tools/magic_bitboard_generator)
add_subdirectory(tools/perft)
add_subdirectory(tools/pgn_parser)
add_subdirectory(tools/table_generator)
add_subdirectory(tools/test_game_filter)

install(EXPORT ChessTargets
    FILE ChessTargets.cmake
    NAMESPACE Chess::
    DESTINATION lib/cmake/Chess
)

include(WriteBasicConfigVersionFile)

write_basic_config_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ChessConfigVersion.cmake"
    VERSION 1.0.0
    COMPATIBILITY SameMajorVersion
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ChessConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/ChessConfig.cmake"
    INSTALL_DESTINATION lib/cmake/Chess
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/ChessConfigVersion.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/ChessConfig.cmake"
    DESTINATION lib/cmake/Chess
)
