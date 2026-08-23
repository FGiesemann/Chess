# Chess Project - chess_game

add_library(chess_game)
add_library(Chess::game ALIAS chess_game)

target_sources(chess_game
    PUBLIC
        FILE_SET HEADERS
            BASE_DIRS include
            FILES
                include/chessgame/cursor.h
                include/chessgame/game.h
                include/chessgame/metadata.h
                include/chessgame/pgn.h
                include/chessgame/san.h
                include/chessgame/tree.h
                include/chessgame/types.h
    PRIVATE
        src/cursor.cpp
        src/game.cpp
        src/metadata.cpp
        src/pgn.cpp
        src/san.cpp
        src/tree.cpp
        src/types.cpp
)

target_compile_features(chess_game PUBLIC cxx_std_23)
enable_exception_handling(chess_game)
target_link_libraries(chess_game PUBLIC Chess::core)
add_compiler_warnings(chess_game)
add_optimization_settings(chess_game)

install(TARGETS chess_game
    EXPORT ChessTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/chess_game
    COMPONENT chess_game
)

if(BUILD_DOCUMENTATION)
    set(CHESS_GAME_DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/doc/api")
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile 
        @ONLY
    )

    add_custom_command(
        OUTPUT ${CHESS_GAME_DOXYGEN_OUTPUT_DIR}/html/index.html
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        DEPENDS 
            ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
            ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            $<TARGET_PROPERTY:chess_game,HEADER_SET_HEADERS>
            $<TARGET_PROPERTY:chess_game,SOURCES>
        COMMAND_EXPAND_LISTS
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating documentation with Doxygen"
        VERBATIM
    )

    add_custom_target(chess_game_docs ALL DEPENDS ${CHESS_GAME_DOXYGEN_OUTPUT_DIR}/html/index.html)
    if (TARGET docs)
        add_dependencies(docs chess_game_docs)
    endif()

    install(DIRECTORY ${CHESS_GAME_DOXYGEN_OUTPUT_DIR}/html DESTINATION share/doc/chess_game
        PATTERN "*.map" EXCLUDE
        PATTERN "*.md5" EXCLUDE
    )
endif()

if (BUILD_TESTING)
    add_subdirectory(test)
endif()
