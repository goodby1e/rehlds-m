cmake_minimum_required(VERSION 3.23)
include(FetchContent)

set(PROJECT_NAME "strformat")
project(${PROJECT_NAME})

add_library(${PROJECT_NAME} INTERFACE)
add_library(StrFormat::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

FetchContent_Declare(
  FmtLib
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        9.1.0
)

set(FMT_SYSTEM_HEADERS ON)
FetchContent_MakeAvailable(FmtLib)

target_link_libraries(${PROJECT_NAME} INTERFACE
  fmt::fmt
)

target_include_directories(${PROJECT_NAME} INTERFACE
  "include"
)

target_sources(${PROJECT_NAME} INTERFACE
  "include/strformat/format.h"
)
