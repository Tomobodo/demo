set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR X86)

set(XWIN_PATH "$ENV{HOME}/.xwin")
set(CRINKLER_EXE "$ENV{HOME}/tools/Crinkler.exe")

set(CMAKE_C_COMPILER clang-cl)
set(CMAKE_CXX_COMPILER clang-cl)

# 32-bit target — Crinkler 2.3 ne supporte pas les COFF x64 générés par clang
set(CMAKE_C_FLAGS_INIT   "--target=i686-pc-windows-msvc")
set(CMAKE_CXX_FLAGS_INIT "--target=i686-pc-windows-msvc")

set(XWIN_LIB      "${XWIN_PATH}/sdk/lib/um/x86")
set(XWIN_CRT_LIB  "${XWIN_PATH}/crt/lib/x86")
set(XWIN_UCRT_LIB "${XWIN_PATH}/sdk/lib/ucrt/x86")

set(CMAKE_EXE_LINKER_FLAGS_INIT
    "/LIBPATH:\"${XWIN_LIB}\" /LIBPATH:\"${XWIN_CRT_LIB}\" /LIBPATH:\"${XWIN_UCRT_LIB}\""
)

set(CMAKE_LINKER "${CMAKE_CURRENT_LIST_DIR}/crinkler-wrapper.sh")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

add_compile_options(
    /imsvc${XWIN_PATH}/sdk/include/um
    /imsvc${XWIN_PATH}/sdk/include/ucrt
    /imsvc${XWIN_PATH}/sdk/include/shared
    /imsvc${XWIN_PATH}/crt/include
    -fno-addrsig
)
