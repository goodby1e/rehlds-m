cmake_minimum_required(VERSION 3.23)

set(PROJECT_NAME "string")
project(${PROJECT_NAME})

add_library(${PROJECT_NAME} INTERFACE)
add_library(CppUtils::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

FetchContent_Declare(
  FmtLib
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        9.1.0
)

set(FMT_SYSTEM_HEADERS ON)
FetchContent_MakeAvailable(FmtLib)

target_link_libraries(${PROJECT_NAME} INTERFACE
  $<$<OR:$<PLATFORM_ID:Linux>,$<PLATFORM_ID:Darwin>>:c>
  fmt::fmt
)

target_include_directories(${PROJECT_NAME} INTERFACE
  "include"
)

target_sources(${PROJECT_NAME} INTERFACE
  "include/cpputils/cstring.h"
  "include/cpputils/format.h"
  "include/cpputils/string.h"
  "include/cpputils/string_const.h"
  "src/cstring.cpp"
  "src/string.cpp"
)

setup_unit_tests("${PROJECT_NAME}_tests" LIBRARIES CppUtils::${PROJECT_NAME} SOURCES
  "test/test_cstring.cpp"
  "test/test_string.cpp"
)
