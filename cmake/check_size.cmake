get_filename_component(BIN_NAME "${TARGET_PATH}" NAME)

if(EXISTS "${TARGET_PATH}")
	file(SIZE "${TARGET_PATH}" FILE_SIZE)

	if(FILE_SIZE GREATER MAX_SIZE)
		math(EXPR OVER "${FILE_SIZE} - ${MAX_SIZE}")
		message(FATAL_ERROR
				"\n"
				"-----------------------------------------------------------\n"
				" [!] SIZE LIMIT EXCEEDED\n"
				" Target:   ${BIN_NAME}\n"
				" Size:     ${FILE_SIZE} bytes\n"
				" Limit:    ${MAX_SIZE} bytes\n"
				" Overby:   ${OVER} bytes\n"
				"-----------------------------------------------------------"
		)
	else()
		math(EXPR REMAINING "${MAX_SIZE} - ${FILE_SIZE}")
		message(STATUS "${BIN_NAME}: ${FILE_SIZE} bytes OK (${REMAINING} bytes left)")
	endif()
else()
	message(FATAL_ERROR "Could not find target file: ${TARGET_PATH}")
endif()