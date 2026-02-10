add_executable(${TARGET_NAME} WIN32)

target_compile_options(
		${TARGET_NAME} PRIVATE
		/std:c++latest
		/GS-
		/Oi
		/Os
		$<$<CONFIG:RelWithDebInfo>:/Od /Z7 /DDEBUG>
		$<$<CONFIG:Release>:/EHa- /Zl /O2 /Gw /Gy>
)

target_link_options(
		${TARGET_NAME} PRIVATE
		/ENTRY:entry
		/NODEFAULTLIB
)

if (USE_CRINKLER)
	target_link_options(
			${TARGET_NAME} PRIVATE
			/TINYHEADER
			/TINYIMPORT
			/COMPMODE:VERYSLOW
			/ORDERTRIES:1000
	)
endif()

target_link_libraries(
		${TARGET_NAME} PRIVATE
		kernel32.lib
		user32.lib
		gdi32.lib
		dwmapi.lib
)