set(TARGET asset_embedder)
add_executable(${TARGET})

target_sources(
        ${TARGET} PRIVATE
        main.cpp
        font.cpp
        ${ROOT_SOURCE_DIR}/src/engine/font.cpp
)

set_target_properties(
        ${TARGET} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
)

add_subdirectory(${ROOT_SOURCE_DIR}/external/pugixml pugixml)

target_link_libraries(
        ${TARGET} PRIVATE
        pugixml
)

target_include_directories(
        ${TARGET} PRIVATE
        ${ROOT_SOURCE_DIR}/include
        ${ROOT_SOURCE_DIR}/external/pugixml/src
)