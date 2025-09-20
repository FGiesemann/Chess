qt_add_executable(testapp
    main.cpp
)

target_link_libraries(testapp
    PRIVATE
    Qt6::Widgets
    ChessGui
)

set_target_properties(testapp PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
