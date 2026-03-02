set(TARGET asset_embedder)
add_executable(${TARGET} main.cpp)

set_target_properties(
        ${TARGET} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
)