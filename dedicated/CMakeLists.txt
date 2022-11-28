#--------------------------------------------------------------------------------------------
#                    Copyright © Valve Corporation. All rights reserved.
#--------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.23)

project("hlds" DESCRIPTION "HLDS Launcher")
find_package(Boost 1.80.0 REQUIRED)

set(PROJECT_NAME_INTERFACE "${PROJECT_NAME}_interface")
add_library(${PROJECT_NAME_INTERFACE} INTERFACE)

target_sources(${PROJECT_NAME_INTERFACE} INTERFACE
  "src/command_line.cpp"
  "src/command_line.h"
  "src/console/text_console.cpp"
  "src/console/text_console.h"
  "src/dedicated_exports.cpp"
  "src/dedicated.cpp"
  "src/dedicated.h"
  "src/sleep.h"

  # Boost
  $<$<NOT:$<BOOL:$<TARGET_PROPERTY:ENABLE_EXCEPTIONS>>>:
    "src/throw_exception.cpp"
  >

  # Platform Windows
  $<$<PLATFORM_ID:Windows>:
    "src/console/text_console_windows.cpp"
    "src/console/text_console_windows.h"
  >

  # Platform Linux, Darwin
  $<$<OR:$<PLATFORM_ID:Linux>,$<PLATFORM_ID:Darwin>>:
    "src/console/text_console_unix.cpp"
    "src/console/text_console_unix.h"
  >
)

target_include_directories(${PROJECT_NAME_INTERFACE} INTERFACE
  "include"
  "src"
)

target_link_libraries(${PROJECT_NAME_INTERFACE} INTERFACE
  Boost::boost
  ReHLDS::common

  # Windows
  $<$<PLATFORM_ID:Windows>:
    WS2_32.Lib
  >

  # Linux and Darwin
  $<$<OR:$<PLATFORM_ID:Linux>,$<PLATFORM_ID:Darwin>>:
    c
    $<$<BOOL:${LINK_STATIC_GCC}>:-static-libgcc>
    $<$<BOOL:${LINK_STATIC_STDCPP}>:-static-libstdc++>
  >

  # Intel LLVM Compiler
  $<$<STREQUAL:"${CMAKE_CXX_COMPILER_ID}","IntelLLVM">:
    $<$<BOOL:${NO_INTEL_LIB}>:-no-intel-lib>
    $<IF:$<BOOL:${LINK_STATIC_INTEL}>,-static-intel,-shared-intel>
  >
)

add_executable(${PROJECT_NAME} "src/main.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME_INTERFACE})

set_target_properties(${PROJECT_NAME} PROPERTIES
  POSITION_INDEPENDENT_CODE OFF
  VS_GLOBAL_ROOTNAMESPACE "${PROJECT_NAME}"
  RUNTIME_OUTPUT_DIRECTORY "${DEFAULT_OUTPUT_DIR}"
)

if(WIN32)
  configure_file("resource.rc.in" "${PROJECT_SOURCE_DIR}/resource.rc" @ONLY)
  target_sources(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/resource.rc")
  target_compile_definitions("${PROJECT_NAME}" PRIVATE _CONSOLE)
elseif(UNIX AND NOT APPLE)
  set_target_properties(${PROJECT_NAME} PROPERTIES
    SUFFIX "_linux"
  )

  if(NOT LINK_STATIC_GCC)
    add_custom_command(
      TARGET ${PROJECT_NAME} POST_BUILD
      WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
      COMMAND sh -c "cp \$(ldd ${PROJECT_NAME}_linux | grep 'libgcc_s.so' | grep -E -o '/.+\\s') . > /dev/null 2>&1 || (exit 0)"
      VERBATIM
    )
  endif()

  if(NOT LINK_STATIC_STDCPP)
    add_custom_command(
      TARGET ${PROJECT_NAME} POST_BUILD
      WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
      COMMAND sh -c "cp \$(ldd ${PROJECT_NAME}_linux | grep 'libstdc++.so' | grep -E -o '/.+\\s') . > /dev/null 2>&1 || (exit 0)"
      VERBATIM
    )
  endif()

  if(NOT NO_INTEL_LIB AND NOT LINK_STATIC_INTEL)
    add_custom_command(
      TARGET ${PROJECT_NAME} POST_BUILD
      WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
      COMMAND sh -c "cp \$(ldd ${PROJECT_NAME}_linux | grep 'libintlc.so' | grep -E -o '/.+\\s') . > /dev/null 2>&1 || (exit 0)"
      VERBATIM
    )
  endif()
endif()

setup_target_compile_definitions(${PROJECT_NAME})
setup_target_compile_options(${PROJECT_NAME})
setup_target_link_options(${PROJECT_NAME})
setup_target_code_analysis(${PROJECT_NAME})

if(BUILD_UNIT_TESTS)
  setup_unit_tests("${PROJECT_NAME}_tests" LIBRARIES ${PROJECT_NAME_INTERFACE} SOURCES
    "test/test_command_line.cpp"
  )
endif()
