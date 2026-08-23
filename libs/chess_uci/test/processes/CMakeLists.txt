function(add_test_binary TARGET_NAME SOURCE_FILE)
    add_executable(${TARGET_NAME} ${SOURCE_FILE})
    
    set_target_properties(${TARGET_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_binaries
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR}/test_binaries
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_BINARY_DIR}/test_binaries
    )
    target_compile_options(${TARGET_NAME}
        PRIVATE
            "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
    )
    enable_exception_handling(${TARGET_NAME})
endfunction()

# "Engine Simulations"
add_test_binary(test_echo helpers/test_echo.cpp)
add_test_binary(test_immediate_exit helpers/test_immediate_exit.cpp)
add_test_binary(test_hang helpers/test_hang.cpp)
add_test_binary(test_slow_start helpers/test_slow_start.cpp)
add_test_binary(test_line_echo helpers/test_line_echo.cpp)
add_test_binary(test_crash helpers/test_crash.cpp)
add_test_binary(test_output_flood helpers/test_output_flood.cpp)
add_test_binary(test_working_dir helpers/test_working_dir.cpp)
if(UNIX)
    add_test_binary(test_zombie helpers/test_zombie.cpp)
endif()

# The process handling tests
add_executable(chess_uci_processhandling_tests
    src/test_engine_process.cpp
)
target_link_libraries(chess_uci_processhandling_tests
    PRIVATE
        Chess::uci
        Catch2::Catch2WithMain
)
target_compile_definitions(chess_uci_processhandling_tests PRIVATE
    TEST_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}/test_binaries"
)
target_compile_options(chess_uci_processhandling_tests
    PRIVATE
        "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
)
enable_exception_handling(chess_uci_processhandling_tests)
add_optimization_settings(chess_uci_processhandling_tests)

add_dependencies(chess_uci_processhandling_tests
    test_echo
    test_immediate_exit
    test_hang
    test_slow_start
    test_line_echo
    test_crash
    test_output_flood
    test_working_dir
)

if(UNIX)
    add_dependencies(chess_uci_processhandling_tests test_zombie)
endif()

catch_discover_tests(chess_uci_processhandling_tests PROPERTIES TIMEOUT 60)
