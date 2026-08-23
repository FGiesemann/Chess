# Chess Project - chess_engine

add_library(chess_engine)
add_library(Chess::engine ALIAS chess_engine)

target_sources(chess_engine
    PUBLIC
        FILE_SET HEADERS
            BASE_DIRS include
            FILES
                include/chessengine/chess_engine.h
                include/chessengine/config.h
                include/chessengine/evaluation.h
                include/chessengine/logger.h
                include/chessengine/test_engine.h
                include/chessengine/types.h
                include/chessengine/uci_adapter.h
    PRIVATE
        src/chessengine/chess_engine.cpp
        src/chessengine/config.cpp
        src/chessengine/evaluation.cpp
        src/chessengine/logger.cpp
        src/chessengine/test_engine.cpp
        src/chessengine/types.cpp
        src/chessengine/uci_adapter.cpp
)
add_compiler_warnings(chess_engine)
add_optimization_settings(chess_engine)
target_compile_features(chess_engine PUBLIC cxx_std_23)
enable_exception_handling(chess_engine)
target_link_libraries(chess_engine
    PUBLIC
        Chess::core
        Chess::uci
)

add_executable(maat
    src/app/main.cpp
)
add_compiler_warnings(maat)
add_optimization_settings(maat)
target_compile_features(maat PUBLIC cxx_std_23)
target_link_libraries(maat PRIVATE chess_engine)
enable_exception_handling(maat)
install(TARGETS chess_engine
    EXPORT ChessTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/chess_engine
    COMPONENT chess_engine
)

if(BUILD_DOCUMENTATION)
    set(CHESS_ENGINE_DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/doc/api")
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile 
        @ONLY
    )

    add_custom_command(
        OUTPUT ${CHESS_ENGINE_DOXYGEN_OUTPUT_DIR}/html/index.html
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        DEPENDS 
            ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
            ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            $<TARGET_PROPERTY:chess_engine,HEADER_SET_HEADERS>
            $<TARGET_PROPERTY:chess_engine,SOURCES>
        COMMAND_EXPAND_LISTS
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating documentation with Doxygen"
        VERBATIM
    )

    add_custom_target(chess_engine_docs ALL DEPENDS ${CHESS_ENGINE_DOXYGEN_OUTPUT_DIR}/html/index.html)
    if (TARGET docs)
        add_dependencies(docs chess_engine_docs)
    endif()

    install(DIRECTORY ${CHESS_ENGINE_DOXYGEN_OUTPUT_DIR}/html DESTINATION share/doc/${PROJECT_NAME}
        PATTERN "*.map" EXCLUDE
        PATTERN "*.md5" EXCLUDE
    )
endif()

if (BUILD_TESTING)
    add_subdirectory(test)
endif()
