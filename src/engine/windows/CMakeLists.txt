add_executable(${PROJECT_NAME} WIN32)

set_target_properties(
        ${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
)

target_link_options(
        ${PROJECT_NAME} PRIVATE
        /ENTRY:entry
        /NODEFAULTLIB
)

target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        kernel32.lib
        user32.lib
        gdi32.lib
        dwmapi.lib
        $<$<CONFIG:RelWithDebInfo>:comctl32.lib>
)

set(COMPILE_OPTIONS
        /std:c++latest
        /GS-
        /Oi
        /arch:AVX2
        /Qvec-report:2
        /fp:fast
        $<$<CONFIG:RelWithDebInfo>:/Od /DDEBUG >
        $<$<CONFIG:Release>:/EHa-s-c- /Zl /O2 /Gw /Gy>
)

target_compile_options(
        ${PROJECT_NAME} PRIVATE
        ${COMPILE_OPTIONS}
)

target_compile_options(
        ${PROJECT_NAME}_module PRIVATE
        ${COMPILE_OPTIONS}
)

target_sources(
        ${PROJECT_NAME} PRIVATE
        entry.cpp
        $<$<CONFIG:RelWithDebInfo>:${CMAKE_CURRENT_SOURCE_DIR}/debug_tools.cpp>
)

target_sources(
        ${PROJECT_NAME}_module PRIVATE
        maths.cpp
        memory.cpp
)

if (HOTRELOAD)
    set(LIB_NAME ${PROJECT_NAME}_lib)

    add_compile_definitions(
            HOTRELOAD
            HOTRELOAD_LIB_NAME="${LIB_NAME}.dll"
            HOTRELOAD_LIB_LOCKED_NAME="${LIB_NAME}_locked.dll"
    )

    add_library(${LIB_NAME} SHARED)
    target_link_libraries(${LIB_NAME} PRIVATE ${PROJECT_NAME}_module)

    target_sources(
            ${LIB_NAME} PRIVATE
            demo.def
    )

    set_target_properties(
            ${LIB_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
    )
else ()
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_module)
endif ()

if (USE_CRINKLER)
    target_link_options(
            ${PROJECT_NAME} PRIVATE
            /TINYHEADER
            /TINYIMPORT
            /OPT:NOICF
            /LTCG:NOSTATUS
            /COMPMODE:VERYSLOW
            /ORDERTRIES:1000
            /LARGEADDRESSAWARE:NO
            /CRINKLER
            /PRIORITY:NORMAL
            /UNSAFEIMPORT
            /TRANSFORM:CALLS
            /NOINITIALIZERS
            /TRUNCATEFLOATS:16
            /OVERRIDEALIGNMENTS
            /UNALIGNCODE
            /REPORT:report.html
            /PROGRESSGUI
    )

    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND}
                -D "TARGET_PATH=$<TARGET_FILE:${PROJECT_NAME}>"
                -D "MAX_SIZE=${MAX_SIZE}"
                -P "${PROJECT_SOURCE_DIR}/cmake/check_size.cmake"
                WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
                COMMENT "Checking binary size"
        )
    endif ()
endif ()