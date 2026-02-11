project(
        gif_tool
        VERSION 0.1.0
        LANGUAGES CXX
)

add_executable(${PROJECT_NAME})

add_subdirectory(src/tool_gif)
add_subdirectory(src/common)
add_subdirectory(src/demo)

target_compile_options(
        ${PROJECT_NAME} PRIVATE
        /nologo
        /EHsc
        $<$<CONFIG:Debug>:/DDEBUG>
)

target_link_options(
        ${PROJECT_NAME} PRIVATE
        /nologo
        $<$<CONFIG:Debug>:/DEBUG>
)