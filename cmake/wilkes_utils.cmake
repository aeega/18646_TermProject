# on msvc, avx2 includes avx and fma
set(WILKES_C_CXX_MSVC_OPTIONS
    "/bigobj"
    "/permissive-"
    "/arch:AVX2"
    "/D_CRT_SECURE_NO_WARNINGS")

set(WILKES_C_CXX_GCC_OPTIONS
    "-mavx"
    "-mavx2" 
    "-mfma")

set(WILKES_C_CXX_CLANG_OPTIONS
    "-mavx"
    "-mavx2" 
    "-mfma")

##-----------------------------------------------------------------------------
function(wilkes_configure_c_cxx this_target)
    target_compile_features(${this_target} PUBLIC cxx_std_17 c_std_99)
    target_compile_definitions(${this_target}
                               PUBLIC
                               _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING)
    target_compile_options(${this_target}
                            PRIVATE
                            $<$<COMPILE_LANG_AND_ID:CXX,MSVC>:${WILKES_C_CXX_MSVC_OPTIONS}>
                            $<$<COMPILE_LANG_AND_ID:C,MSVC>:${WILKES_C_CXX_MSVC_OPTIONS}>

                            $<$<COMPILE_LANG_AND_ID:CXX,GCC>:${WILKES_C_CXX_GCC_OPTIONS}>
                            $<$<COMPILE_LANG_AND_ID:C,GCC>:${WILKES_C_CXX_GCC_OPTIONS}>

                            $<$<COMPILE_LANG_AND_ID:CXX,GCC>:${WILKES_C_CXX_CLANG_OPTIONS}>
                            $<$<COMPILE_LANG_AND_ID:C,GCC>:${WILKES_C_CXX_CLANG_OPTIONS}>)
    set_target_properties(${this_target} PROPERTIES
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF
        POSITION_INDEPENDENT_CODE ON
    )
endfunction()


##-----------------------------------------------------------------------------
function(wilkes_configure_openmp this_target)
    if (OpenMP_FOUND)
        message(STATUS "OpenMP found, adding to target: ${this_target}")
        target_link_libraries(${this_target} PRIVATE OpenMP::OpenMP_CXX)
    else()
        message(STATUS "OpenMP not found, disabling from target: ${this_target}")
    endif()
endfunction()

##-----------------------------------------------------------------------------
function(wilkes_define_library this_target)
    set(options USE_OPENMP)
    set(one_value_args)
    set(multi_value_args SOURCES HEADERS DEPENDENCIES PRIVATE_DEPS)
    cmake_parse_arguments(WILKES_CURLIB 
                          "${options}"
                          "${one_value_args}"
                          "${multi_value_args}"
                          ${ARGN})
    add_library(${this_target} STATIC
        ${WILKES_CURLIB_HEADERS}
        ${WILKES_CURLIB_SOURCES})

    wilkes_configure_c_cxx(${this_target})
    if (WILKES_CURLIB_USE_OPENMP)
        wilkes_configure_openmp(${this_target})
    endif()

    set_property(TARGET ${this_target}
        PROPERTY PUBLIC_HEADER
        ${WILKES_CURLIB_HEADERS})
    if (WILKES_CURLIB_DEPENDENCIES)
        target_link_libraries(${this_target} PUBLIC ${WILKES_CURLIB_DEPENDENCIES})
    endif()
    if (WILKES_CURLIB_PRIVATE_DEPS)
        target_link_libraries(${this_target} PRIVATE ${WILKES_CURLIB_PRIVATE_DEPS})
    endif()
    target_include_directories(${this_target}
        PUBLIC
         $<BUILD_INTERFACE:${wilkes_SOURCE_DIR}>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    add_library(wilkes::${this_target} ALIAS ${this_target})

    install(TARGETS ${this_target}
        EXPORT wilkes
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include/wilkes/${this_target}
    )
endfunction()

##-----------------------------------------------------------------------------
function(wilkes_define_executable this_target)
    set(options USE_OPENMP)
    set(one_value_args)
    set(multi_value_args SOURCES DEPENDENCIES)
    cmake_parse_arguments(WILKES_CURLIB 
                          "${options}"
                          "${one_value_args}"
                          "${multi_value_args}"
                          ${ARGN})
    add_executable(${this_target}
        ${WILKES_CURLIB_SOURCES})

    wilkes_configure_c_cxx(${this_target})
    if (WILKES_CURLIB_USE_OPENMP)
        wilkes_configure_openmp(${this_target})
    endif()

    if (WILKES_CURLIB_DEPENDENCIES)
        target_link_libraries(${this_target} PUBLIC ${WILKES_CURLIB_DEPENDENCIES})
    endif()
    target_include_directories(${this_target}
        PUBLIC
         $<BUILD_INTERFACE:${wilkes_SOURCE_DIR}>
         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )

    add_executable(wilkes::${this_target} ALIAS ${this_target})

    install(TARGETS ${this_target}
        EXPORT wilkes
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        PUBLIC_HEADER DESTINATION include/wilkes/${this_target}
    )
endfunction()

