cmake_minimum_required(VERSION 3.30)

project(
        demo_template
        VERSION 0.2.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

set(PLATFORM_SRC ${PROJECT_SOURCE_DIR}/platforms/${CMAKE_SYSTEM_NAME}/src)

include_directories(
        include
)

include(cmake/demo/demo.cmake)

if (NOT CMAKE_SYSTEM_NAME STREQUAL "wasm")
    include(cmake/gif_export.cmake)
endif ()
