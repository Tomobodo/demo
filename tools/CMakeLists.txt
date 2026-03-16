cmake_minimum_required(VERSION 3.30)

project(
        host_tools
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-stdlib=libc++)
    add_link_options(-stdlib=libc++)
endif ()

if (NOT ROOT_SOURCE_DIR)
    set(ROOT_SOURCE_DIR ${CMAKE_SOURCE_DIR})
endif ()

add_subdirectory(asset_embedder)

