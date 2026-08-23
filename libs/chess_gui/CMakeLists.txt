# Chess Project - chess_gui

find_package(Qt6 REQUIRED COMPONENTS Core Widgets SvgWidgets)
qt_standard_project_setup()

qt_add_library(chess_gui STATIC)
add_library(Chess::gui ALIAS chess_gui)

set(CHESS_GUI_SOURCES
    src/ChessboardController.cpp
    src/ChessboardError.cpp
    src/ChessboardWidget.cpp
    src/GraphicsItems.cpp
    src/MoveTreeModel.cpp
    src/MoveTreeWidget.cpp
    src/PieceSet.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/data/pieces/pieces.qrc    
)
target_sources(chess_gui
    PUBLIC
        FILE_SET HEADERS
            BASE_DIRS include
            FILES
                include/chess_gui/ChessboardController.h
                include/chess_gui/ChessboardError.h
                include/chess_gui/ChessboardWidget.h
                include/chess_gui/GraphicsItems.h
                include/chess_gui/MoveTreeModel.h
                include/chess_gui/MoveTreeWidget.h
                include/chess_gui/PieceSet.h
    PRIVATE
        ${CHESS_GUI_SOURCES}
)
set_target_properties(chess_gui
    PROPERTIES
        AUTOMOC ON
        AUTORCC ON
)   
target_link_libraries(chess_gui
    PUBLIC
        Chess::core
        Chess::game
        Qt6::Widgets
        Qt6::SvgWidgets
)
add_compiler_warnings(chess_gui)
add_optimization_settings(chess_gui)
enable_exception_handling(chess_gui)

install(TARGETS chess_gui
    EXPORT ChessTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT chess_gui
)

if(BUILD_DOCUMENTATION)
    set(CHESS_GUI_DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/doc/api")
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile 
        @ONLY
    )

    add_custom_command(
        OUTPUT ${CHESS_GUI_DOXYGEN_OUTPUT_DIR}/html/index.html
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        DEPENDS 
            ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
            ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            $<TARGET_PROPERTY:chess_gui,HEADER_SET_HEADERS>
            ${CHESS_GUI_SOURCES}
        COMMAND_EXPAND_LISTS
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating documentation with Doxygen"
        VERBATIM
    )

    add_custom_target(chess_gui_docs ALL DEPENDS ${CHESS_GUI_DOXYGEN_OUTPUT_DIR}/html/index.html)
    if (TARGET docs)
        add_dependencies(docs chess_gui_docs)
    endif()

    install(DIRECTORY ${CHESS_GUI_DOXYGEN_OUTPUT_DIR}/html DESTINATION share/doc/${PROJECT_NAME}
        PATTERN "*.map" EXCLUDE
        PATTERN "*.md5" EXCLUDE
    )
endif()

add_subdirectory(testapp)
