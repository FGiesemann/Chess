add_executable(bitmap_tool)

target_sources(bitmap_tool
    PRIVATE
        bitmap_tool.cpp
)

target_link_libraries(bitmap_tool
    PRIVATE
    Chess::core
    Chess::core_io
)

enable_utf8_output(bitmap_tool)

add_optimization_settings(bitmap_tool)

install(TARGETS bitmap_tool
    EXPORT ChessTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
