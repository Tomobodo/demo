set(MODULE_NAME ${PROJECT_NAME}_module)
set(TARGET gif)

add_executable(${TARGET} entry.cpp maths.cpp)

set_target_properties(${TARGET} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

target_include_directories(
        ${TARGET} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(
        ${TARGET} PRIVATE
        ${MODULE_NAME}
)