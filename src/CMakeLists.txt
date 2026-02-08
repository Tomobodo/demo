if (BUILD_WASM)
	set (PLATFORM_SRCS_DIR wasm32)
else ()
	set (PLATFORM_SRCS_DIR win32)
endif ()

target_sources(
		${TARGET_NAME} PRIVATE
		${PLATFORM_SRCS_DIR}/main.cpp
		${PLATFORM_SRCS_DIR}/maths.cpp
)

add_subdirectory(common)

