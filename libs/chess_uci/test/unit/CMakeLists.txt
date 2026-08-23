add_executable(chess_uci_unittests
    src/engine_handler_callback_test.cpp
    src/engine_handler_parsing_test.cpp
    src/gui_handler_callback_test.cpp
    src/gui_handler_parsing_test.cpp
    src/uci_move_conversion_test.cpp
    src/uci_move_matcher_test.cpp
    src/uci_move_parser_test.cpp

    helper/EngineProcessMock.cpp
)
target_include_directories(chess_uci_unittests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
target_compile_definitions(chess_uci_unittests PRIVATE
    TEST_BINARY_DIR="${CMAKE_BINARY_DIR}/test/processes/test_binaries"
)
enable_exception_handling(chess_uci_unittests)
add_dependencies(chess_uci_unittests
    test_echo
)

if (WIN32)
    target_sources(chess_uci_unittests 
        PRIVATE
            src/unicode_conversion_win_test.cpp
    )
endif()

add_compiler_warnings(chess_uci_unittests)
add_optimization_settings(chess_uci_unittests)

target_link_libraries(chess_uci_unittests
    PRIVATE
        Chess::uci
        Chess::core_io
        Catch2::Catch2WithMain
)

catch_discover_tests(chess_uci_unittests PROPERTIES TIMEOUT 20)
