project(
        gif_tool
        VERSION 0.1.0
        LANGUAGES CXX
)

add_executable(${PROJECT_NAME})

add_subdirectory(src/tool_gif ${PROJECT_NAME})
add_subdirectory(src/common ${PROJECT_NAME}/common)
add_subdirectory(src/windows ${PROJECT_NAME}/windows)

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

target_include_directories(
        ${PROJECT_NAME} PRIVATE
        src
)