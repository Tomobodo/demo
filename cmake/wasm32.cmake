set(TARGET_NAME demo_wasm)

add_executable(${TARGET_NAME})

set_target_properties(
		${TARGET_NAME} PROPERTIES
		PREFIX ""
		SUFFIX ".wasm"
)

target_compile_options(
		${TARGET_NAME} PRIVATE
		-ffreestanding
		$<$<CONFIG:Release>: -O3 -Os>
		$<$<CONFIG:Debug>: -g>
		-std=c++23
)

target_link_options(
		${TARGET_NAME} PRIVATE
		-flto
		-nostdlib
		-Wl,--no-entry
		-Wl,--export-all
		-Wl,--allow-undefined
		#-Wl,--import-memory
		-Wl,--initial-memory=1310720
)

configure_file(
		${CMAKE_CURRENT_SOURCE_DIR}/templates/wasm32/index.html
		${CMAKE_CURRENT_BINARY_DIR}/index.html
)

if (USE_WASM_STRIP)
	find_program(WASM_STRIP wasm-strip)

	if (WASM_STRIP)
		add_custom_command(
				TARGET ${TARGET_NAME} POST_BUILD
				COMMAND ${WASM_STRIP} $<TARGET_FILE:${TARGET_NAME}>
				COMMENT "Stripping Wasm binary..."
		)
	endif ()
endif ()