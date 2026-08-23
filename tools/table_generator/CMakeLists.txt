add_executable(table_generator)

target_sources(table_generator
    PRIVATE
        main.cpp
        bigmap.cpp
        table_gen.cpp
        patterns.cpp
)

target_link_libraries(table_generator
    PRIVATE
        Chess::core
        Chess::core_io
)

add_optimization_settings(table_generator)

install(TARGETS table_generator
    EXPORT ChessTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT tools
)
