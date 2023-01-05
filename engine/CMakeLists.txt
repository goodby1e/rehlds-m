#--------------------------------------------------------------------------------------------
#                    Copyright © Valve Corporation. All rights reserved.
#--------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.23)

set(PROJECT_NAME        "engine")
set(PROJECT_DESCRIPTION "HLDS Engine")

project(${PROJECT_NAME}
  VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}
  DESCRIPTION "${PROJECT_DESCRIPTION}"
  HOMEPAGE_URL "${PROJECT_URL}"
  LANGUAGES "CXX"
)

add_library(${PROJECT_NAME} SHARED)
add_library(ReHLDS::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
