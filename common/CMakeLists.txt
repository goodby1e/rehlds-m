#--------------------------------------------------------------------------------------------
#                    Copyright © Valve Corporation. All rights reserved.
#--------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.23)

set(PROJECT_NAME "common")
project(${PROJECT_NAME})

add_library(${PROJECT_NAME} INTERFACE)
add_library(ReHLDS::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE
  "include"
)

target_link_libraries(${PROJECT_NAME} INTERFACE
  CppUtils::singleton
  CppUtils::system
)

target_sources(${PROJECT_NAME} INTERFACE
  "include/common/hlds_module.h"
  "include/common/interface.h"
  "include/common/interfaces/dedicated_exports.h"
  "include/common/interfaces/dedicated_serverapi.h"
  "include/common/interfaces/filesystem.h"
  "include/common/interfaces/system_base.h"
  "include/common/interfaces/system_module.h"
  "include/common/interfaces/vgui_module.h"
  "include/common/object_container.h"
  "include/common/object_list.h"
  "include/common/platform.h"
  "src/interface.cpp"
  "src/object_list.cpp"
)

setup_unit_tests("${PROJECT_NAME}_tests" LIBRARIES ReHLDS::${PROJECT_NAME} SOURCES
  "test/test_object_list.cpp"
)
