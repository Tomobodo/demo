target_sources(
        ${TARGET} PRIVATE
        ${PLATFORM_SRC}/demo/entry.cpp
)

set_target_properties(
        ${TARGET} PROPERTIES
        WIN32_EXECUTABLE TRUE
)

target_compile_options(
        ${TARGET} PRIVATE
        /std:c++latest
        /GS-
        /Oi
        /Os
        $<$<CONFIG:RelWithDebInfo>:/Od /Z7 /DDEBUG>
        $<$<CONFIG:Release>:/EHa-s-c- /Zl /O2 /Gw /Gy>
)

target_link_options(
        ${TARGET} PRIVATE
        /ENTRY:entry
        /NODEFAULTLIB
)

if (USE_CRINKLER)
    target_link_options(
            ${TARGET} PRIVATE
            /TINYHEADER
            /TINYIMPORT
            /COMPMODE:VERYSLOW
            /ORDERTRIES:1000
    )
endif ()

target_link_libraries(
        ${TARGET} PRIVATE
        kernel32.lib
        user32.lib
        gdi32.lib
        dwmapi.lib
)

if (HOTRELOAD)
    target_compile_definitions(
            ${TARGET_EXE} PRIVATE
            HOTRELOAD_LIB_NAME="${TARGET_LIB}.dll"
            HOTRELOAD_LIB_LOCKED_NAME="${TARGET_LIB}_locked.dll"
    )

    set(TARGET ${TARGET_LIB})
    add_library(${TARGET} SHARED)

    include(GenerateExportHeader)

    generate_export_header(
            ${TARGET}
            BASE_NAME ${TARGET}
            EXPORT_FILE_NAME ${PROJECT_SOURCE_DIR}/include/generated/${TARGET}_export.hpp
    )
endif ()