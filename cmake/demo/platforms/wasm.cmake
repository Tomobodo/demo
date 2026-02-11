set(INITIAL_MEMORY_PAGES 350)
set(MAX_MEMORY_PAGES 350)

math(EXPR INITIAL_MEMORY "${INITIAL_MEMORY_PAGES} * 65536")
math(EXPR MAX_MEMORY "${MAX_MEMORY_PAGES} * 65536")

set(OUTPUT_DIST_DIR "${PROJECT_BINARY_DIR}/dist")

set_target_properties(
		${PROJECT_NAME} PROPERTIES
		PREFIX ""
		SUFFIX ".wasm"
		RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIST_DIR}
)

target_compile_options(
		${PROJECT_NAME} PRIVATE
		-ffreestanding
		$<$<CONFIG:Release>: -O3 -Os>
		$<$<CONFIG:Debug>: -g -O0>
		-std=c++23
		-matomics
		-mbulk-memory
)

target_link_options(
		${PROJECT_NAME} PRIVATE
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
				TARGET ${PROJECT_NAME} POST_BUILD
				COMMAND ${WASM_STRIP} $<TARGET_FILE:${PROJECT_NAME}>
				COMMENT "Stripping Wasm binary..."
		)
	endif ()
endif ()

# Generate shell

if(HOTRELOAD)
	set(SHELL_HR_SCRIPT "<script src='scripts/hot-reload.js'></script>")
endif ()

set(SHELL_SRC "${PROJECT_SOURCE_DIR}/platforms/wasm/shell")
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
