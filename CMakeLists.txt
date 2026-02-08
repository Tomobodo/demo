cmake_minimum_required(VERSION 4.2)

project(
		demo
		VERSION 0.1.0
		LANGUAGES CXX
)

set(TARGET_NAME demo)

option(BUILD_WASM "Build for wasm32 platform" OFF)
option(USE_WASM_STRIP "Strip final wasm file" OFF)
option(USE_CRINKLER "Compress final executable with CRINKLER" OFF)
option(HOTRELOAD "Enabled hot reload" OFF)

set(CRINKLER_PATH "Crinkler.exe" CACHE FILEPATH "Path to Crinkler executable")
set(BUFFER_WIDTH 640 CACHE STRING "Buffer width")
set(BUFFER_HEIGHT 480 CACHE STRING "Buffer height")

if (BUILD_WASM)
	include(cmake/wasm32.cmake)
else ()
	include(cmake/win32.cmake)
endif ()

add_subdirectory(src)

target_compile_definitions(${TARGET_NAME} PRIVATE
		FULLSCREEN
		BUFFER_WIDTH=${BUFFER_WIDTH}
		BUFFER_HEIGHT=${BUFFER_HEIGHT}
		SHOW_FPS
)
