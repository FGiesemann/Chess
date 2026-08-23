# Chess Project - chess_uci

add_library(chess_uci)
add_library(Chess::uci ALIAS chess_uci)

target_sources(chess_uci
    PUBLIC
        FILE_SET HEADERS
            BASE_DIRS include
            FILES
                include/chessuci/engine_handler.h
                include/chessuci/engine_process_unix.h
                include/chessuci/engine_process_win.h
                include/chessuci/engine_process.h
                include/chessuci/gui_handler.h
                include/chessuci/move.h
                include/chessuci/process_factory.h
                include/chessuci/protocol.h
                include/chessuci/uci_handler.h
    PRIVATE
        src/engine_handler.cpp
        src/engine_process.cpp
        src/gui_handler.cpp
        src/move.cpp
        src/process_factory.cpp
        src/protocol.cpp
        src/string_conversion.h
        src/uci_handler.cpp
)
if(WIN32)
    target_sources(chess_uci PRIVATE src/engine_process_win.cpp)
    target_compile_definitions(chess_uci
        PUBLIC
            CHESSUCI_WINDOWS
            UNICODE
            _UNICODE
            WIN32_LEAN_AND_MEAN
            NOMINMAX
    )
    target_compile_options(chess_uci PUBLIC "/utf-8")
elseif(APPLE)
    target_sources(chess_uci PRIVATE src/engine_process_unix.cpp)
    target_compile_definitions(chess_uci PUCLIC CHESSUCI_MACOS CHESSUCI_UNIX)
elseif(UNIX)
    target_sources(chess_uci PRIVATE src/engine_process_unix.cpp)
    target_compile_definitions(chess_uci PUBLIC CHESSUCI_LINUX CHESSUCI_UNIX)
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()
target_compile_features(chess_uci PUBLIC cxx_std_23)
enable_exception_handling(chess_uci)
target_link_libraries(chess_uci PUBLIC Chess::core)
add_compiler_warnings(chess_uci)
add_optimization_settings(chess_uci)

install(TARGETS chess_uci
    EXPORT ChessTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/chessuci
    COMPONENT chess_uci
)

if(BUILD_DOCUMENTATION)
    set(CHESS_UCI_DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/doc/api")
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile 
        @ONLY
    )

    add_custom_command(
        OUTPUT ${CHESS_UCI_DOXYGEN_OUTPUT_DIR}/html/index.html
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        DEPENDS 
            ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
            ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            $<TARGET_PROPERTY:chess_uci,HEADER_SET_HEADERS>
            $<TARGET_PROPERTY:chess_uci,SOURCES>
        COMMAND_EXPAND_LISTS
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating documentation with Doxygen"
        VERBATIM
    )

    add_custom_target(chess_uci_docs ALL DEPENDS ${CHESS_UCI_DOXYGEN_OUTPUT_DIR}/html/index.html)
    if (TARGET docs)
        add_dependencies(docs chess_uci_docs)
    endif()

    install(DIRECTORY ${CHESS_UCI_DOXYGEN_OUTPUT_DIR}/html DESTINATION share/doc/${PROJECT_NAME}
        PATTERN "*.map" EXCLUDE
        PATTERN "*.md5" EXCLUDE
    )
endif()

if (BUILD_TESTING)
    add_subdirectory(test)
endif()
