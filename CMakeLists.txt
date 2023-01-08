#------------------------------------------------------------------------------------------------
#                      Copyright © Valve Corporation. All rights reserved.
#------------------------------------------------------------------------------------------------
# Compile with MSVC:
#   cmake --preset x86-windows-msvc-release && cmake --build --preset x86-windows-msvc-release
#
# Compile with Clang-CL:
#   cmake --preset x86-windows-clang-release && cmake --build --preset x86-windows-clang-release
#
# Compile with GCC:
#   cmake --preset x86-linux-gcc-release && cmake --build --preset x86-linux-gcc-release
#
# Compile with Clang:
#   cmake --preset x86-linux-clang-release && cmake --build --preset x86-linux-clang-release
#
# Compile with Intel C++ Compiler:
#   cmake --preset x86-linux-intel-release && cmake --build --preset x86-linux-intel-release
#------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.23)

#------------------------------------------------------------------------------------------------
# Project
#------------------------------------------------------------------------------------------------
project(        "rehlds-m"
  VERSION       "1.2.0.0"
  DESCRIPTION   "Reverse-engineered HLDS"
  HOMEPAGE_URL  "https://github.com/goodby1e/rehlds-m"
  LANGUAGES     "CXX"
)

#------------------------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------------------------
option(ENABLE_RTTI              "Enable RTTI support"                                       OFF )
option(ENABLE_EXCEPTIONS        "Enable exception handling table generation"                OFF )
option(CLANG_TIDY_CODE_ANALYSIS "Enable Clang-Tidy code analysis"                           OFF )
option(CPPCHECK_CODE_ANALYSIS   "Enable Cppcheck code analysis"                             OFF )
option(IWYU_ANALYSIS            "Enable Include What You Use analysis"                      OFF )
option(GCC_STATIC_ANALYSIS      "Enable GCC Static code analysis"                           OFF )
option(MICROSOFT_CODE_ANALYSIS  "Enable Microsoft code analysis"                            OFF )
option(PVS_STUDIO_CODE_ANALYSIS "Enable PVS-Studio code analysis"                           OFF )
option(SANITIZE_ADDRESS         "Enable AddressSanitizer"                                   OFF )
option(SANITIZE_UNDEFINED       "Enable UndefinedBehaviorSanitizer (Linux only)"            OFF )
option(BUILD_UNIT_TESTS         "Build unit tests"                                          OFF )
option(USE_LINKER_GOLD          "Use the Gold linker (with GCC)"                            ON  )
option(USE_LINKER_LLD           "Use the LLD linker (with Clang)"                           ON  )
option(LINK_STATIC_GCC          "Static linking with the libgcc library"                    OFF )
option(LINK_STATIC_STDCPP       "Static linking with the libstdc++ library"                 OFF )
option(LINK_STATIC_INTEL        "Link Intel provided libraries statically"                  OFF )
option(NO_INTEL_LIB             "Restrict linking of all Intel specific libraries"          OFF )
option(PUBLISH_ARTIFACTS        "Create a 'publish' directory with build artifacts"         OFF )
#------------------------------------------------------------------------------------------------

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(FetchContent)
include(GetGitRevisionDescription)

get_git_head_revision(GIT_REFSPEC GIT_SHA1)
string(SUBSTRING "${GIT_SHA1}" 0 7 GIT_SHA1_TRUNC)

FetchContent_Declare(
  PVS_CMakeModule
  GIT_REPOSITORY https://github.com/viva64/pvs-studio-cmake-module.git
  GIT_TAG        master
)

FetchContent_Declare(
  GoogleTest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)

if(BUILD_UNIT_TESTS)
  include(CTest)
endif()

include(Config)
include(UnitTests)
include(CodeAnalysis)
include(${CMAKE_HOST_SYSTEM_NAME})

add_subdirectory("libs/cpputils")
add_subdirectory("common")
add_subdirectory("dedicated")
