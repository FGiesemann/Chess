# Chess Project - chess_core

add_library(chess_core)
add_library(Chess::core ALIAS chess_core)

target_sources(chess_core
    PUBLIC
    FILE_SET HEADERS
        BASE_DIRS include
        FILES
            include/chesscore/bitboard.h
            include/chesscore/bitmap.h
            include/chesscore/board.h
            include/chesscore/chesscore.h
            include/chesscore/epd.h
            include/chesscore/fen.h
            include/chesscore/magic.h
            include/chesscore/move.h
            include/chesscore/perft.h
            include/chesscore/piece.h
            include/chesscore/position_types.h
            include/chesscore/position.h
            include/chesscore/square.h
            include/chesscore/table.h
            include/chesscore/zobrist.h
    PRIVATE
        src/chesscore/bitboard_tables.h
        src/chesscore/bitboard.cpp
        src/chesscore/bitmap.cpp
        src/chesscore/board.cpp
        src/chesscore/chesscore.cpp
        src/chesscore/epd.cpp
        src/chesscore/fen.cpp
        src/chesscore/magic.cpp
        src/chesscore/move.cpp
        src/chesscore/perft.cpp
        src/chesscore/piece.cpp
        src/chesscore/position.cpp
        src/chesscore/position_types.cpp
        src/chesscore/square.cpp
        src/chesscore/table.cpp
        src/chesscore/zobrist.cpp
)

target_compile_features(chess_core PUBLIC cxx_std_23)
enable_exception_handling(chess_core)
add_compiler_warnings(chess_core)
add_optimization_settings(chess_core)

install(TARGETS chess_core
    EXPORT ChessTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT chess_core
)

add_library(chess_core_io)
add_library(Chess::core_io ALIAS chess_core_io)

target_sources(chess_core_io
    PUBLIC
    FILE_SET HEADERS
        BASE_DIRS include
        FILES
            include/chesscore_io/bitboard_io.h
            include/chesscore_io/bitmap_io.h
            include/chesscore_io/move_io.h
            include/chesscore_io/piece_io.h
            include/chesscore_io/position_io.h
            include/chesscore_io/square_io.h
    PRIVATE
        src/chesscore_io/bitboard_io.cpp
        src/chesscore_io/bitmap_io.cpp
        src/chesscore_io/move_io.cpp
        src/chesscore_io/piece_io.cpp
        src/chesscore_io/position_io.cpp
        src/chesscore_io/square_io.cpp
)

target_compile_features(chess_core_io PUBLIC cxx_std_23)
target_link_libraries(chess_core_io PUBLIC chess_core)
add_compiler_warnings(chess_core_io)
add_optimization_settings(chess_core_io)
enable_utf8_output(chess_core_io)
enable_exception_handling(chess_core_io)

install(TARGETS chess_core_io
    EXPORT ChessTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/chess_core_io
    COMPONENT chess_core
)

if(BUILD_DOCUMENTATION)
    set(CHESS_CORE_DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/doc/api")
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile 
        @ONLY
    )

    add_custom_command(
        OUTPUT ${CHESS_CORE_DOXYGEN_OUTPUT_DIR}/html/index.html
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        DEPENDS 
            ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
            ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            $<TARGET_PROPERTY:chess_core,HEADER_SET_HEADERS>
            $<TARGET_PROPERTY:chess_core,SOURCES>
        COMMAND_EXPAND_LISTS
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating doxygen documentation for chess_core"
        VERBATIM
    )

    add_custom_target(chess_core_docs ALL DEPENDS ${CHESS_CORE_DOXYGEN_OUTPUT_DIR}/html/index.html)
    if (TARGET docs)
        add_dependencies(docs chess_core_docs)
    endif()

    install(DIRECTORY ${CHESS_CORE_DOXYGEN_OUTPUT_DIR}/html DESTINATION share/doc/chess_core
        PATTERN "*.map" EXCLUDE
        PATTERN "*.md5" EXCLUDE
    )
endif()

if (BUILD_TESTING)
    add_subdirectory(test)
endif()
