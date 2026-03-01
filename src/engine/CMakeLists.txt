add_subdirectory(${SYSTEM_NAME})

target_sources(
        ${PROJECT_NAME}_module PRIVATE
        timeline.cpp
        color.cpp
        maths.cpp
)

