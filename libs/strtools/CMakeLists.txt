cmake_minimum_required(VERSION 3.23)

set(PROJECT_NAME "strtools")
project(${PROJECT_NAME})

add_library(${PROJECT_NAME} INTERFACE)
add_library(StrTools::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} INTERFACE
  $<$<OR:$<PLATFORM_ID:Linux>,$<PLATFORM_ID:Darwin>>:c>
)

target_include_directories(${PROJECT_NAME} INTERFACE
  "include"
)

target_sources(${PROJECT_NAME} INTERFACE
  "include/strtools/const.h"
  "include/strtools/cstring.h"
  "include/strtools/string.h"
  "src/cstring.cpp"
  "src/string.cpp"
)

setup_unit_tests("${PROJECT_NAME}_tests" LIBRARIES StrTools::${PROJECT_NAME} SOURCES
  "test/test_cstring.cpp"
  "test/test_string.cpp"
)
