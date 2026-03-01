set(TARGET ${PROJECT_NAME}_module)

add_library(${TARGET} OBJECT EXCLUDE_FROM_ALL)

target_sources(
        ${TARGET} PRIVATE
        scene_a.cpp
        scene_b.cpp
        rotoz.cpp
        demo.cpp
        plasma.cpp
)
