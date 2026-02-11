option(USE_WASM_STRIP "Strip final wasm file" OFF)
option(USE_CRINKLER "Compress final executable with CRINKLER" OFF)
option(HOTRELOAD "Enabled hot reload" OFF)

set(MAX_SIZE 4096 CACHE STRING "Maximum binary size in bytes")

set(BUFFER_WIDTH 640 CACHE STRING "Buffer width")
set(BUFFER_HEIGHT 480 CACHE STRING "Buffer height")

project(
        demo
        VERSION 0.1.0
        LANGUAGES CXX
)

set(PLATFORM_CMAKE_PATH ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}/platforms/${CMAKE_SYSTEM_NAME}.cmake)

if (NOT EXISTS ${PLATFORM_CMAKE_PATH})
    return()
endif ()

add_executable(${PROJECT_NAME})

include(${PLATFORM_CMAKE_PATH})

add_subdirectory(src/demo ${PROJECT_NAME})
add_subdirectory(src/common ${PROJECT_NAME}/common)

target_sources(
        ${PROJECT_NAME} PRIVATE
        ${PROJECT_SOURCE_DIR}/platforms/${CMAKE_SYSTEM_NAME}/src/demo/entry.cpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        FULLSCREEN
        BUFFER_WIDTH=${BUFFER_WIDTH}
        BUFFER_HEIGHT=${BUFFER_HEIGHT}
        SHOW_FPS
)

if (USE_CRINKLER)
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            -D "TARGET_PATH=$<TARGET_FILE:${PROJECT_NAME}>"
            -D "MAX_SIZE=${MAX_SIZE}"
            -P "${PROJECT_SOURCE_DIR}/cmake/check_size.cmake"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Checking binary size"
    )
endif()
