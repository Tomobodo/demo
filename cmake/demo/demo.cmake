option(USE_WASM_STRIP "Strip final wasm file" OFF)
option(USE_CRINKLER "Compress final executable with CRINKLER" OFF)
option(HOTRELOAD "Enable hot reload" OFF)
option(SHOW_FPS "Enable fps counter" OFF)

set(MAX_SIZE 4096 CACHE STRING "Maximum binary size in bytes")

set(BUFFER_WIDTH 640 CACHE STRING "Buffer width")
set(BUFFER_HEIGHT 480 CACHE STRING "Buffer height")

project(
        demo
        VERSION 0.1.0
        LANGUAGES CXX
)

set(TARGET_EXE ${PROJECT_NAME})
set(TARGET_LIB ${PROJECT_NAME}_lib)

set(TARGET ${TARGET_EXE})

set(PLATFORM_CMAKE_PATH ${PROJECT_SOURCE_DIR}/cmake/${TARGET}/platforms/${CMAKE_SYSTEM_NAME}.cmake)

if (NOT EXISTS ${PLATFORM_CMAKE_PATH})
    return()
endif ()

add_executable(${TARGET})

target_compile_definitions(
        ${TARGET} PRIVATE
        #FULLSCREEN
        PLATFORM_${CMAKE_SYSTEM_NAME}
        $<$<BOOL:${HOTRELOAD}>:HOTRELOAD>
)

# Before this include, the target is the executable shell, platform specific

include(${PLATFORM_CMAKE_PATH})

# From here, the target is the demo itseld, either a separate library or in the executable

add_subdirectory(src/demo ${TARGET})
add_subdirectory(src/engine ${TARGET}/engine)

target_compile_definitions(
        ${TARGET} PRIVATE
        BUFFER_WIDTH=${BUFFER_WIDTH}
        BUFFER_HEIGHT=${BUFFER_HEIGHT}
        PLATFORM_${CMAKE_SYSTEM_NAME}
        $<$<BOOL:${HOTRELOAD}>:HOTRELOAD>
        $<$<BOOL:${SHOW_FPS}>:SHOW_FPS>
)

if (USE_CRINKLER)
    add_custom_command(
            TARGET ${TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            -D "TARGET_PATH=$<TARGET_FILE:${TARGET}>"
            -D "MAX_SIZE=${MAX_SIZE}"
            -P "${PROJECT_SOURCE_DIR}/cmake/check_size.cmake"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Checking binary size"
    )
endif ()
