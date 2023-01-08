cmake_minimum_required(VERSION 3.23)

set(PROJECT_NAME "atexit")
project(${PROJECT_NAME})

add_library(${PROJECT_NAME} INTERFACE)
add_library(CppUtils::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} INTERFACE
  $<$<OR:$<PLATFORM_ID:Linux>,$<PLATFORM_ID:Darwin>>:
    c
  >
)

target_include_directories(${PROJECT_NAME} INTERFACE
  "include"
)

target_sources(${PROJECT_NAME} INTERFACE
  "include/cpputils/at_exit.h"
  "src/at_exit.cpp"
)
