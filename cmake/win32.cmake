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

if (USE_CRINKLER)
	set(CMAKE_CXX_LINK_EXECUTABLE "${CRINKLER_PATH} \
            /NODEFAULTLIB \
            /ENTRY:entry \
           	/TINYHEADER \
		    /TINYIMPORT \
		    /COMPMODE:VERYSLOW \
		    /ORDERTRIES:1000 \
            <OBJECTS> /OUT:<TARGET> <LINK_LIBRARIES>")
else()
	target_link_options(
			${TARGET_NAME} PRIVATE
			/ENTRY:entry
			/NODEFAULTLIB
	)
endif()

target_link_libraries(
		${TARGET_NAME} PRIVATE
		kernel32.lib
		user32.lib
		gdi32.lib
		dwmapi.lib
)