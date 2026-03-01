cmake_minimum_required(VERSION 3.30)

option(USE_WASM_STRIP "Strip final wasm file" OFF)
option(USE_CRINKLER "Compress final executable with CRINKLER" OFF)
option(HOTRELOAD "Enable hot reload" OFF)
option(SHOW_FPS "Enable fps counter" OFF)

set(MAX_SIZE 4096 CACHE STRING "Maximum binary size in bytes")

set(BUFFER_WIDTH 640 CACHE STRING "Buffer width")
set(BUFFER_HEIGHT 480 CACHE STRING "Buffer height")

project(
        demo
        VERSION 0.2.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

include_directories(
        include
)

string(TOLOWER "${CMAKE_SYSTEM_NAME}" SYSTEM_NAME)

add_compile_definitions(
        BUFFER_WIDTH=${BUFFER_WIDTH}
        BUFFER_HEIGHT=${BUFFER_HEIGHT}
        PLATFORM_${SYSTEM_NAME}
        $<$<BOOL:${SHOW_FPS}>:SHOW_FPS>
)

add_subdirectory(tools)
add_subdirectory(src)
