cmake_minimum_required(VERSION 3.30)

option(BUILD_WASM "Build for wasm32 platform" OFF)
option(USE_WASM_STRIP "Strip final wasm file" OFF)
option(USE_CRINKLER "Compress final executable with CRINKLER" OFF)
option(HOTRELOAD "Enabled hot reload" OFF)

set(MAX_SIZE 4096 CACHE STRING "Maximum binary size in bytes")
set(CRINKLER_PATH "Crinkler.exe" CACHE FILEPATH "Path to Crinkler executable")
set(BUFFER_WIDTH 640 CACHE STRING "Buffer width")
set(BUFFER_HEIGHT 480 CACHE STRING "Buffer height")

project(
		demo
		VERSION 0.1.0
		LANGUAGES CXX
)

set(TARGET_NAME demo)

if (CMAKE_SYSTEM_NAME STREQUAL "wasm")
	include(cmake/wasm.cmake)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	include(cmake/windows.cmake)
endif ()

add_subdirectory(src)

target_compile_definitions(${TARGET_NAME} PRIVATE
		FULLSCREEN
		BUFFER_WIDTH=${BUFFER_WIDTH}
		BUFFER_HEIGHT=${BUFFER_HEIGHT}
		SHOW_FPS
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
	add_custom_command(
			TARGET ${TARGET_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND}
			-D "TARGET_PATH=$<TARGET_FILE:${TARGET_NAME}>"
			-D "MAX_SIZE=${MAX_SIZE}"
			-P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/check_size.cmake"
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			COMMENT "Checking binary size"
	)
endif()