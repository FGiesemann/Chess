option(ENABLE_LTO "Enable Link Time Optimization" ON)
set(ARCH_LEVEL "native" CACHE STRING "x86-64 Architecture Level: generic, v2, v3, v4, native")

function(add_optimization_settings TARGET_NAME)
    get_target_property(TARGET_TYPE ${TARGET_NAME} TYPE)

    if (TARGET_TYPE STREQUAL "STATIC_LIBRARY" OR 
        TARGET_TYPE STREQUAL "SHARED_LIBRARY" OR 
        TARGET_TYPE STREQUAL "MODULE_LIBRARY")
        set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "d")
    endif()
    
    set(_ARCH_STR $<IF:$<STREQUAL:${ARCH_LEVEL},native>,native,x86-64-${ARCH_LEVEL}>)

    target_compile_options(${TARGET_NAME} PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-march=${_ARCH_STR}>
        $<$<CXX_COMPILER_ID:Clang>:-march=${_ARCH_STR}>

        $<$<CXX_COMPILER_ID:MSVC>:$<$<STREQUAL:${ARCH_LEVEL},v3>:/arch:AVX2>>
        $<$<CXX_COMPILER_ID:MSVC>:$<$<STREQUAL:${ARCH_LEVEL},v4>:/arch:AVX512>>
    )

    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(${TARGET_NAME} PRIVATE
            # LTO
            $<$<AND:$<CXX_COMPILER_ID:GNU>,$<BOOL:${ENABLE_LTO}>>:-flto=auto>
            $<$<AND:$<CXX_COMPILER_ID:Clang>,$<BOOL:${ENABLE_LTO}>>:-flto=thin>
            $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<BOOL:${ENABLE_LTO}>>:/GL;/W4;/permissive->

            $<$<CXX_COMPILER_ID:GNU>:-O3>          

            $<$<CXX_COMPILER_ID:Clang>:-O3>

            $<$<CXX_COMPILER_ID:MSVC>:/O2>
            $<$<CXX_COMPILER_ID:MSVC>:/Oi>
            $<$<CXX_COMPILER_ID:MSVC>:/Ot>
            $<$<CXX_COMPILER_ID:MSVC>:/Gt>
        )

        target_link_options(${TARGET_NAME} PRIVATE
            $<$<AND:$<CXX_COMPILER_ID:GNU>,$<BOOL:${ENABLE_LTO}>>:-flto=auto>

            $<$<AND:$<CXX_COMPILER_ID:Clang>,$<BOOL:${ENABLE_LTO}>>:-flto=thin>
            $<$<AND:$<CXX_COMPILER_ID:Clang>,$<BOOL:${ENABLE_LTO}>>:-fuse-ld=lld>
            
            $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<BOOL:${ENABLE_LTO}>>:/LTCG>
            $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<BOOL:${ENABLE_LTO}>>:/INCREMENTAL:NO>
        )
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${TARGET_NAME} PRIVATE
            $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Og;-g>
        )
    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        target_compile_options(${TARGET_NAME} PRIVATE 
            $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-O3;-g;-fno-omit-frame-pointer>
            $<$<CXX_COMPILER_ID:MSVC>:/O2;/Oi;/Ot;/Gt>
        )
    endif()
endfunction()

function(enable_exception_handling TARGET_NAME)
    target_compile_options(${TARGET_NAME} PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/EHsc>)
endfunction()

function(enable_utf8_output TARGET_NAME)
    target_compile_options(${TARGET_NAME} PUBLIC "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endfunction()
