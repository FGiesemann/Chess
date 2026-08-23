# Chess Project - maat chess engine

add_executable(maat
    src/main.cpp
)
add_compiler_warnings(maat)
add_optimization_settings(maat)
target_compile_features(maat PUBLIC cxx_std_23)
target_link_libraries(maat PRIVATE Chess::engine)
enable_exception_handling(maat)

install(TARGETS maat
    EXPORT ChessTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT chess_engine
)
