set(INITIAL_MEMORY_PAGES 350)
set(MAX_MEMORY_PAGES 350)

math(EXPR INITIAL_MEMORY "${INITIAL_MEMORY_PAGES} * 65536")
math(EXPR MAX_MEMORY "${MAX_MEMORY_PAGES} * 65536")

add_executable(${TARGET_NAME})

set(OUTPUT_DIST_DIR "${CMAKE_CURRENT_BINARY_DIR}/dist")

set_target_properties(
		${TARGET_NAME} PROPERTIES
		OUTPUT_NAME "demo"
		PREFIX ""
		SUFFIX ".wasm"
		RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIST_DIR}
)

target_compile_options(
		${TARGET_NAME} PRIVATE
		-ffreestanding
		$<$<CONFIG:Release>: -O3 -Os>
		$<$<CONFIG:Debug>: -g -O0>
		-std=c++23
		-matomics
		-mbulk-memory
)

target_link_options(
		${TARGET_NAME} PRIVATE
		-nostdlib
		-Wl,--no-entry
		-Wl,--export-all
		-Wl,--allow-undefined
		-Wl,--shared-memory
		-Wl,--import-memory
		-Wl,--initial-memory=${INITIAL_MEMORY}
		-Wl,--max-memory=${MAX_MEMORY}
		-Wl,--threads=24
		# disabling every possible link time optimisations in debug
		# for faster hotreload
		$<$<CONFIG:Release>: -flto>
		$<$<CONFIG:Debug>: -Wl,--no-merge-data-segments>
		$<$<CONFIG:Debug>: -Wl,--no-gc-sections>
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

if (HOTRELOAD)
	add_custom_command(
			TARGET ${TARGET_NAME} POST_BUILD
			COMMAND curl -s http://localhost:8082/ --output /dev/null --max-time 1 || cmake -E true
	)
endif ()

# Generate shell

set(SHELL_SRC "${CMAKE_CURRENT_SOURCE_DIR}/platforms/wasm/shell")
set(SHELL_DST "${OUTPUT_DIST_DIR}")

file(GLOB_RECURSE ALL_FILES RELATIVE "${SHELL_SRC}" "${SHELL_SRC}/*")

set(TEXT_EXTENSIONS ".html" ".js" ".css" ".json" ".txt")

foreach(FILE_PATH IN LISTS ALL_FILES)
	get_filename_component(FILE_EXT "${FILE_PATH}" LAST_EXT)

	set(SRC_FILE "${SHELL_SRC}/${FILE_PATH}")
	set(DST_FILE "${SHELL_DST}/${FILE_PATH}")

	if(FILE_EXT IN_LIST TEXT_EXTENSIONS)
		# Traitement template (remplacement des ${VARIABLES})
		configure_file("${SRC_FILE}" "${DST_FILE}" @ONLY)
	else()
		# Copie binaire pure (image, ico, wasm, etc.)
		file(COPY "${SRC_FILE}" DESTINATION "${SHELL_DST}")

		# Note : file(COPY) recrée la structure de dossier mais
		# a tendance à copier le fichier dans un dossier si on ne fait pas gaffe.
		# Pour une précision chirurgicale, on peut utiliser :
		configure_file("${SRC_FILE}" "${DST_FILE}" COPYONLY)
	endif()
endforeach()

#configure_file(
#		${CMAKE_CURRENT_SOURCE_DIR}/platforms/wasm/shell/index.html
#		${OUTPUT_DIST_DIR}/index.html
#)