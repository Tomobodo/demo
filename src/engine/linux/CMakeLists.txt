add_executable(${PROJECT_NAME})

set_target_properties(
  ${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
)


set(COMPILE_OPTIONS
  -fno-exceptions
  -fno-stack-protector
  $<$<CONFIG:Release>:-Oz>
  $<$<CONFIG:Release>:-ffunction-sections>
  $<$<CONFIG:Release>:-fdata-sections>
  $<$<CONFIG:Release>:-fno-asynchronous-unwind-tables>
  $<$<CONFIG:Release>:-fno-unwind-tables>
)

target_compile_options(
  ${PROJECT_NAME}_module PRIVATE
  ${COMPILE_OPTIONS}
)

target_compile_options(
  ${PROJECT_NAME} PRIVATE
  ${COMPILE_OPTIONS}
)

target_sources(
  ${PROJECT_NAME} PRIVATE
  entry.s
  entry.cpp
)

target_sources(
  ${PROJECT_NAME}_module PRIVATE
  memory.cpp
  maths.cpp
)

target_link_options(
  ${PROJECT_NAME} PRIVATE
  -nostdlib
  -Wl,-e,entry
  $<$<CONFIG:Release>:-Wl,--strip-all>
  $<$<CONFIG:Release>:-Wl,--gc-sections>
  $<$<CONFIG:Release>:-Wl,--discard-all>
)

target_link_libraries(
  ${PROJECT_NAME} PRIVATE
  ${PROJECT_NAME}_module
  X11
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND sstrip $<TARGET_FILE:${PROJECT_NAME}>
)
endif()
