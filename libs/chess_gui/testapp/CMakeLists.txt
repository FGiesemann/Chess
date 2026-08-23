qt_add_executable(testapp
    main.cpp
    TestAppWindow.h
    TestAppWindow.cpp
)

target_link_libraries(testapp
    PRIVATE
        Qt6::Widgets
        Chess::gui
)

add_optimization_settings(testapp)
