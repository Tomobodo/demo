target_sources(
		${PROJECT_NAME} PRIVATE
		${CMAKE_SYSTEM_NAME}/main.cpp
		${CMAKE_SYSTEM_NAME}/maths.cpp
)

target_include_directories(
		${PROJECT_NAME} PRIVATE
		.
)

add_subdirectory(common)
