add_executable(${PROJECT_NAME})

set_target_properties(
  ${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
)

target_link_options(
  ${PROJECT_NAME} PRIVATE
  -nostdlib
  -Wl,-e,entry
)

target_compile_options(
  ${PROJECT_NAME}_module PRIVATE
  -fno-exceptions
  -fno-stack-protector
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

target_link_libraries(
  ${PROJECT_NAME} PRIVATE
  ${PROJECT_NAME}_module
)
