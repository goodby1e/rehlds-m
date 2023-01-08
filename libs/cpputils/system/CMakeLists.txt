cmake_minimum_required(VERSION 3.23)

set(PROJECT_NAME "system")
project(${PROJECT_NAME})

add_library(${PROJECT_NAME} INTERFACE)
add_library(CppUtils::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} INTERFACE
  $<$<OR:$<PLATFORM_ID:Linux>,$<PLATFORM_ID:Darwin>>:
    ${CMAKE_DL_LIBS}
  >
)

target_include_directories(${PROJECT_NAME} INTERFACE
  "include"
)

target_sources(${PROJECT_NAME} INTERFACE
  "include/cpputils/system.h"

  # Platform Windows
  $<$<PLATFORM_ID:Windows>:
    "include/cpputils/system_windows.h"
    "src/system_windows.cpp"
  >

  # Platform Linux, Darwin
  $<$<OR:$<PLATFORM_ID:Linux>,$<PLATFORM_ID:Darwin>>:
    "include/cpputils/system_linux.h"
    "src/system_linux.cpp"
  >
)
