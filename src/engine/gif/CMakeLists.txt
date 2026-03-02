set(MODULE_NAME ${PROJECT_NAME}_module)
set(TARGET gif)

add_executable(${TARGET} entry.cpp maths.cpp)

target_include_directories(
        ${TARGET} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(
        ${TARGET} PRIVATE
        ${MODULE_NAME}
)