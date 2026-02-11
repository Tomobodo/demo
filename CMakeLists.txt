cmake_minimum_required(VERSION 3.30)

option(BUILD_WASM "Build for wasm platform" OFF)
option(USE_WASM_STRIP "Strip final wasm file" OFF)
option(USE_CRINKLER "Compress final executable with CRINKLER" OFF)
option(HOTRELOAD "Enabled hot reload" OFF)

set(MAX_SIZE 4096 CACHE STRING "Maximum binary size in bytes")
set(CRINKLER_PATH "Crinkler.exe" CACHE FILEPATH "Path to Crinkler executable")
set(BUFFER_WIDTH 640 CACHE STRING "Buffer width")
set(BUFFER_HEIGHT 480 CACHE STRING "Buffer height")

project(
		demo_template
		VERSION 0.2.0
		LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

include_directories(
		include
)

include(cmake/demo/demo.cmake)

if (NOT CMAKE_SYSTEM_NAME STREQUAL "wasm")
include(cmake/gif_tool.cmake)
endif()
