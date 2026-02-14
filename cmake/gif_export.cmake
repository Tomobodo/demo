project(
        gif_export
        VERSION 0.1.0
        LANGUAGES CXX
)

set(TARGET ${PROJECT_NAME})

add_executable(${TARGET})

add_subdirectory(src/gif_export)
add_subdirectory(src/engine)

target_sources(
        ${TARGET} PRIVATE
        src/demo/scene_a.cpp
)

target_compile_options(
        ${TARGET} PRIVATE
        /nologo
        /EHsc
        $<$<CONFIG:Debug>:/DDEBUG>
)

target_link_options(
        ${TARGET} PRIVATE
        /nologo
        $<$<CONFIG:Debug>:/DEBUG>
)