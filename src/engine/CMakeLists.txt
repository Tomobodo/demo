add_subdirectory(${PLATFORM})

target_sources(
        ${PROJECT_NAME}_module PRIVATE
        timeline.cpp
        color.cpp
        maths.cpp
)

