set(TARGET ${PROJECT_NAME}_module)

add_library(${TARGET} OBJECT EXCLUDE_FROM_ALL)

target_sources(
        ${TARGET} PRIVATE
        scene_a.cpp
        scene_b.cpp
        rotoz.cpp
        demo.cpp
        plasma.cpp
        draw_text.cpp
        segment_sdf.cpp
)

set(GENERATED_DIR ${CMAKE_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_DIR})

set(ASSET_INPUT_DIR ${CMAKE_SOURCE_DIR}/assets)
set(ASSET_OUTPUT_HEADER ${GENERATED_DIR}/assets.hpp)

add_custom_command(
        OUTPUT ${ASSET_OUTPUT_HEADER}
        COMMAND ${ASSET_EMBEDDER_EXE}
        ${CMAKE_SOURCE_DIR}
        ${ASSET_INPUT_DIR}
        ${ASSET_OUTPUT_HEADER}
        DEPENDS ${ASSET_EMBEDDER_EXE}
        ${ASSET_INPUT_DIR}
        COMMENT "Embedding assets..."
        VERBATIM
)

add_custom_target(generate_assets
        DEPENDS ${ASSET_OUTPUT_HEADER}
)

add_dependencies(${TARGET} generate_assets)

target_include_directories(${TARGET} PRIVATE ${GENERATED_DIR})