cmake_minimum_required(VERSION 3.30)

string(TOLOWER "${CMAKE_SYSTEM_NAME}" SYSTEM_NAME)

if (NOT SYSTEM_NAME)
    string(TOLOWER "${CMAKE_HOST_SYSTEM_NAME}" SYSTEM_NAME)
endif ()

option(USE_WASM_STRIP "Strip final wasm file" OFF)
option(USE_CRINKLER "Compress final executable with CRINKLER" OFF)
option(HOTRELOAD "Enable hot reload" OFF)
option(SHOW_FPS "Enable fps counter" OFF)
set(PLATFORM "${SYSTEM_NAME}" CACHE STRING "Target platform, leave empty for host")
set_property(CACHE PLATFORM PROPERTY STRINGS "windows" "wasm32" "gif")

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

add_compile_definitions(
        BUFFER_WIDTH=${BUFFER_WIDTH}
        BUFFER_HEIGHT=${BUFFER_HEIGHT}
        PLATFORM_${PLATFORM}
        $<$<BOOL:${SHOW_FPS}>:SHOW_FPS>
)

# Building tools for assets and code generation
include(ExternalProject)

set(HOST_TOOLS_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/tools)
set(ASSET_EMBEDDER_EXE ${HOST_TOOLS_BINARY_DIR}/asset_embedder${CMAKE_HOST_EXECUTABLE_SUFFIX})

ExternalProject_Add(
        host_tools_build
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/tools
        BINARY_DIR ${HOST_TOOLS_BINARY_DIR}
        #        STAMP_DIR ${HOST_TOOLS_BINARY_DIR}/.stamps
        INSTALL_COMMAND ""
        BUILD_ALWAYS TRUE
        BUILD_BYPRODUCTS ${ASSET_EMBEDDER_EXE}
        CMAKE_ARGS
        -GNinja
        -DROOT_SOURCE_DIR=${CMAKE_SOURCE_DIR}
        -DCMAKE_TOOLCHAIN_FILE=
        -DCMAKE_SYSTEM_NAME=${CMAKE_HOST_SYSTEM_NAME}
        -DCMAKE_SYSTEM_PROCESSOR=${CMAKE_HOST_SYSTEM_PROCESSOR}
        -DCMAKE_BUILD_TYPE=Release
)

add_subdirectory(tools EXCLUDE_FROM_ALL)
add_subdirectory(src)