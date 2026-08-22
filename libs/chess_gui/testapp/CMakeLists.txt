qt_add_executable(testapp
    main.cpp
    TestAppWindow.h
    TestAppWindow.cpp
)

target_link_libraries(testapp
    PRIVATE
    Qt6::Widgets
    ChessGui
)

add_optimization_settings(testapp)

# set_target_properties(testapp PROPERTIES
#     WIN32_EXECUTABLE ON
#     MACOSX_BUNDLE ON
# )
