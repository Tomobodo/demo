set(CMAKE_SYSTEM_NAME windows)
set(CMAKE_SYSTEM_PROCESSOR ${MSVC_ARCH})
set(CMAKE_NINJA_FORCE_RESPONSE_FILE 1 CACHE INTERNAL "")

if(NOT DEFINED MSVC_ARCH)
    set(MSVC_ARCH x64)
endif()

if(NOT DEFINED _MSVC_ENV_IMPORTED)
    set(_MSVC_ENV_IMPORTED 1)

    set(_VSWHERE "C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe")
    if(NOT EXISTS "${_VSWHERE}")
        message(FATAL_ERROR "vswhere not found: ${_VSWHERE}")
    endif()

    execute_process(
            COMMAND "${_VSWHERE}" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
            OUTPUT_VARIABLE _VS_INSTALL
            OUTPUT_STRIP_TRAILING_WHITESPACE
            RESULT_VARIABLE _VSWHERE_RC
    )

    if(NOT _VSWHERE_RC EQUAL 0 OR _VS_INSTALL STREQUAL "")
        message(FATAL_ERROR "No Visual Studio install found")
    endif()

    file(TO_CMAKE_PATH "${_VS_INSTALL}" _VS_INSTALL)

    # MSVC
    file(GLOB _MSVC_VERSIONS "${_VS_INSTALL}/VC/Tools/MSVC/*")
    if(_MSVC_VERSIONS STREQUAL "")
        message(FATAL_ERROR "No MSVC version found in ${_VS_INSTALL}/VC/Tools/MSVC")
    endif()

    list(SORT _MSVC_VERSIONS COMPARE NATURAL)
    list(GET _MSVC_VERSIONS -1 _MSVC_TOOLS_DIR)

    set(_CL_EXE   "${_MSVC_TOOLS_DIR}/bin/Host${MSVC_ARCH}/${MSVC_ARCH}/cl.exe")

    if (USE_CRINKLER)
        find_program(CRINKLER_PATH
                NAMES Crinkler Crinkler.exe
                PATHS ENV PATH
                NO_CACHE
        )
        set(_LINK_EXE ${CRINKLER_PATH})
    else()
        set(_LINK_EXE "${_MSVC_TOOLS_DIR}/bin/Host${MSVC_ARCH}/${MSVC_ARCH}/link.exe")
        if(NOT EXISTS "${_LINK_EXE}")
            message(FATAL_ERROR "link.exe not found: ${_LINK_EXE}")
        endif()
    endif()

    set(_ARCH_ARG "-arch=${MSVC_ARCH}")

    if(NOT EXISTS "${_CL_EXE}")
        message(FATAL_ERROR "cl.exe not found: ${_CL_EXE}")
    endif()

    set(_VSDEVCMD "${_VS_INSTALL}/Common7/Tools/VsDevCmd.bat")
    if(NOT EXISTS "${_VSDEVCMD}")
        message(FATAL_ERROR "VsDevCmd.bat not found: ${_VSDEVCMD}")
    endif()

    # output vcvars in a file so it can works from both cmd or msys64 zsh
    set(_ENV_TXT "${CMAKE_BINARY_DIR}/_msvc_env.txt")
    set(_ENV_CMD "${CMAKE_BINARY_DIR}/_gen_msvc_env.cmd")

    file(WRITE "${_ENV_CMD}"
            "@echo off\r\n"
            "call \"${_VSDEVCMD}\" ${_ARCH_ARG} \r\n"
            "if errorlevel 1 exit /b 1\r\n"
            "set > \"${_ENV_TXT}\"\r\n"
    )

    execute_process(
            COMMAND cmd.exe /d /s /c "${_ENV_CMD}"
            RESULT_VARIABLE _RC
    )

    if(NOT _RC EQUAL 0)
        message(FATAL_ERROR "failed executing VsDevCmd: ${_VSDEVCMD}")
    endif()

    if(NOT EXISTS "${_ENV_TXT}")
        message(FATAL_ERROR "env file not générated: ${_ENV_TXT}")
    endif()

    # fetch env from generated file instead of stdout
    file(READ "${_ENV_TXT}" _ENV_DUMP)

    string(REPLACE "\r\n" "\n" _ENV_DUMP "${_ENV_DUMP}")
    string(REPLACE "\r"   "\n" _ENV_DUMP "${_ENV_DUMP}")
    string(REPLACE "\n" ";" _ENV_LINES "${_ENV_DUMP}")

    set(_INCLUDE "")
    set(_LIB "")
    set(_LIBPATH "")
    set(_PATH "")

    foreach(_LINE IN LISTS _ENV_LINES)
        if(_LINE MATCHES "^INCLUDE=(.*)$")
            set(_INCLUDE "${CMAKE_MATCH_1}")
        elseif(_LINE MATCHES "^LIB=(.*)$")
            set(_LIB "${CMAKE_MATCH_1}")
        elseif(_LINE MATCHES "^LIBPATH=(.*)$")
            set(_LIBPATH "${CMAKE_MATCH_1}")
        elseif(_LINE MATCHES "^PATH=(.*)$")
            set(_PATH "${CMAKE_MATCH_1}")
        endif()
    endforeach()

    if(_INCLUDE STREQUAL "")
        message(FATAL_ERROR "INCLUDE still empty after VsDevCmd.")
    endif()

    # Add includes and lib directories
    set(_KITS_BASE "C:/Program Files (x86)/Windows Kits/10")

    if(NOT EXISTS "${_KITS_BASE}/Include")
        message(FATAL_ERROR "Windows SDK not found (Windows Kits 10).")
    endif()

    # Find latest Windows SDK version
    file(GLOB _SDK_INCLUDE_VERSIONS "${_KITS_BASE}/Include/*")
    list(SORT _SDK_INCLUDE_VERSIONS COMPARE NATURAL)
    list(GET _SDK_INCLUDE_VERSIONS -1 _SDK_INC_VER_DIR)
    get_filename_component(_SDK_VER "${_SDK_INC_VER_DIR}" NAME)

    set(_SDK_BIN_DIR "${_KITS_BASE}/bin/${_SDK_VER}/${MSVC_ARCH}")
    set(_RC_EXE "${_SDK_BIN_DIR}/rc.exe")
    set(_MT_EXE "${_SDK_BIN_DIR}/mt.exe")

    if(NOT EXISTS "${_RC_EXE}")
        message(FATAL_ERROR "rc.exe not found: ${_RC_EXE}")
    endif()

    set(CMAKE_RC_COMPILER "${_RC_EXE}" CACHE FILEPATH "" FORCE)

    if(EXISTS "${_MT_EXE}")
        set(CMAKE_MT "${_MT_EXE}" CACHE FILEPATH "" FORCE)
    endif()

    if(NOT _PATH STREQUAL "")
        set(ENV{PATH} "${_PATH}")
    endif()

    # /I
    set(_MSVC_INC_FLAGS "")
    string(REPLACE ";" "\n" _tmp "${_INCLUDE}")
    string(REPLACE "\n" ";" _inc_list "${_tmp}")
    foreach(_p IN LISTS _inc_list)
        if(NOT _p STREQUAL "")
            file(TO_CMAKE_PATH "${_p}" _p2)
            string(APPEND _MSVC_INC_FLAGS " /I\"${_p2}\"")
        endif()
    endforeach()

    # /LIBPATH
    set(_MSVC_LIBPATH_FLAGS "")
    foreach(_raw IN ITEMS "${_LIB}" "${_LIBPATH}")
        if(NOT _raw STREQUAL "")
            string(REPLACE ";" "\n" _tmp "${_raw}")
            string(REPLACE "\n" ";" _lib_list "${_tmp}")
            foreach(_p IN LISTS _lib_list)
                if(NOT _p STREQUAL "")
                    file(TO_CMAKE_PATH "${_p}" _p2)
                    string(APPEND _MSVC_LIBPATH_FLAGS " /LIBPATH:\"${_p2}\"")
                endif()
            endforeach()
        endif()
    endforeach()

    message(STATUS "MSVC env imported (${MSVC_ARCH}) via VsDevCmd.bat")
endif()

# Compilers and linker paths
set(CMAKE_C_COMPILER "${_CL_EXE}" CACHE FILEPATH "" FORCE)
set(CMAKE_CXX_COMPILER "${_CL_EXE}" CACHE FILEPATH "" FORCE)
set(CMAKE_LINKER "${_LINK_EXE}" CACHE FILEPATH "" FORCE)

# Inject include/lib paths
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}${_MSVC_INC_FLAGS}"   CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}${_MSVC_INC_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS}${_MSVC_LIBPATH_FLAGS}"    CACHE STRING "" FORCE)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}${_MSVC_LIBPATH_FLAGS}" CACHE STRING "" FORCE)
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}${_MSVC_LIBPATH_FLAGS}" CACHE STRING "" FORCE)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_compile_options(
        /nologo
        /Zc:__cplusplus
        $<$<CONFIG:Debug>:/Od>
        $<$<CONFIG:Debug>:/Zi>
        $<$<CONFIG:RelWithDebInfo>:/Od> # MSVC is super annoying in debug when working with no crt, forcing some flags we can't remove so
        $<$<CONFIG:RelWithDebInfo>:/Zi> # we prefer using RelWithDebInfo with some debug flags instead
        $<$<CONFIG:Release>:/O2>
        $<$<CONFIG:Release>:/DNDEBUG>
)

add_link_options(
        /nologo
        $<$<CONFIG:RelWithDebInfo>:/DEBUG:FULL>
        $<$<CONFIG:Debug>:/DEBUG:FULL>
)

set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)
