if (USE_SMOL AND CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_CXX_LINK_EXECUTABLE
      "${CMAKE_SOURCE_DIR}/cmake/smol-wrapper.sh -o <TARGET> <OBJECTS> <LINK_LIBRARIES>")
  set(SMOL_EXTRA_COMPILE_OPTIONS -fomit-frame-pointer)
endif()

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
  ${SMOL_EXTRA_COMPILE_OPTIONS}
)

target_compile_options(
  ${PROJECT_NAME} PRIVATE
  ${COMPILE_OPTIONS}
  ${SMOL_EXTRA_COMPILE_OPTIONS}
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
  $<$<CONFIG:Release>:-Wl,--strip-all>
  $<$<CONFIG:Release>:-Wl,--gc-sections>
  $<$<CONFIG:Release>:-Wl,--discard-all>
)

target_link_libraries(
  ${PROJECT_NAME} PRIVATE
  ${PROJECT_NAME}_module
  X11
)

if (CMAKE_BUILD_TYPE STREQUAL "Release" AND NOT USE_SMOL)
  add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND sstrip $<TARGET_FILE:${PROJECT_NAME}>
  )
endif()

if (USE_SMOL AND CMAKE_BUILD_TYPE STREQUAL "Release")
  add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND python3 $ENV{HOME}/tools/smol/smoltrunc.py $<TARGET_FILE:${PROJECT_NAME}> $<TARGET_FILE:${PROJECT_NAME}>.trunc
    COMMAND ${CMAKE_COMMAND} -E rename $<TARGET_FILE:${PROJECT_NAME}>.trunc $<TARGET_FILE:${PROJECT_NAME}>
    COMMAND chmod +x $<TARGET_FILE:${PROJECT_NAME}>
  )
endif()
