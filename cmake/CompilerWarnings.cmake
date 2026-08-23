function(add_compiler_warnings TARGET_NAME)
    target_compile_options(${TARGET_NAME} PUBLIC
        $<$<CXX_COMPILER_ID:GNU>:-Wall;-Wextra;-Wpedantic;-Wshadow;-Wformat=2;-Wconversion;-Wnon-virtual-dtor;-Wold-style-cast;-Wcast-align;-Wunused;-Wunreachable-code;-Wnull-dereference;-Wdouble-promotion;-Werror=return-type>
        $<$<CXX_COMPILER_ID:Clang>:-Wall;-Wextra;-Wpedantic;-Wshadow;-Wno-c++98-compat;-Wno-c++98-compat-pedantic;-Wno-pre-c++20-compat-pedantic;-Wformat=2;-Wconversion;-Wnon-virtual-dtor;-Wold-style-cast;-Wcast-align;-Wunused;-Wunreachable-code;-Wnull-dereference;-Wdouble-promotion;-Werror=return-type;-Werror=implicit-function-declaration;-Werror=incompatible-pointer-types>
        $<$<CXX_COMPILER_ID:MSVC>:/W4;/WX>
        $<$<CXX_COMPILER_ID:MSVC>:/permissive->
    )

    # Disable warnings for braced scalar initialization in Clang (erroneous warning for designated initializers)
    target_compile_options(${TARGET_NAME} PUBLIC
        $<$<CXX_COMPILER_ID:Clang>:-Wno-braced-scalar-init>
    )

    # Disable warnings for padded structs
    target_compile_options(${TARGET_NAME} PUBLIC
        $<$<CXX_COMPILER_ID:Clang>:-Wno-padded>
    )

    # Disable warnings for unsafe buffer usage
    target_compile_options(${TARGET_NAME} PUBLIC
        $<$<CXX_COMPILER_ID:Clang>:-Wno-unsafe-buffer-usage>
    )

    # Disable warnings for switch-statements
    target_compile_options(${TARGET_NAME} PUBLIC
        $<$<CXX_COMPILER_ID:Clang>:-Wno-switch-default>
        $<$<CXX_COMPILER_ID:Clang>:-Wno-switch-enum>
        $<$<CXX_COMPILER_ID:Clang>:-Wno-covered-switch-default>
    )

    # Disable C2y extension warnings
    target_compile_options(${TARGET_NAME} PUBLIC
        $<$<CXX_COMPILER_ID:Clang>:-Wno-c2y-extensions>
    )

    target_compile_options(${TARGET_NAME} PUBLIC
        $<$<AND:$<CXX_COMPILER_ID:GNU>,$<NOT:$<BOOL:CMAKE_DEBUG_POSTFIX>>>:-Werror>
        $<$<AND:$<CXX_COMPILER_ID:Clang>,$<NOT:$<BOOL:CMAKE_DEBUG_POSTFIX>>>:-Werror>
    )

    target_compile_options(${TARGET_NAME} PUBLIC
        $<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>
    )
endfunction()
