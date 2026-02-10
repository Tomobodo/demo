option(BUILD_WASM "Build for wasm platform" OFF)
option(USE_WASM_STRIP "Strip final wasm file" OFF)
option(USE_CRINKLER "Compress final executable with CRINKLER" OFF)
option(HOTRELOAD "Enabled hot reload" OFF)

project(
        demo
        VERSION 0.1.0
        LANGUAGES CXX
)

if (CMAKE_SYSTEM_NAME STREQUAL "wasm")
    include(cmake/demo_wasm.cmake)
elseif(CMAKE_SYSTEM_NAME STREQUAL "windows")
    include(cmake/demo_windows.cmake)
endif ()

add_subdirectory(src)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        FULLSCREEN
        BUFFER_WIDTH=${BUFFER_WIDTH}
        BUFFER_HEIGHT=${BUFFER_HEIGHT}
        SHOW_FPS
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            -D "TARGET_PATH=$<TARGET_FILE:${PROJECT_NAME}>"
            -D "MAX_SIZE=${MAX_SIZE}"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/check_size.cmake"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Checking binary size"
    )
endif()
