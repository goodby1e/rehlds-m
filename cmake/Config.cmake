#--------------------------------------------------------------------------------------------
# General CMake configuration
#--------------------------------------------------------------------------------------------

include(ProcessorCount)
ProcessorCount(NCORES)

if(NCORES EQUAL 0)
  set(NCORES 4)
endif()

if(NCORES GREATER 8)
  set(NCORES 8)
endif()

set(ENV{CMAKE_BUILD_PARALLEL_LEVEL} ${NCORES})
set(ENV{CTEST_PARALLEL_LEVEL} ${NCORES})
set(CMAKE_BUILD_PARALLEL_LEVEL ${NCORES})
set(CTEST_BUILD_FLAGS -j${NCORES})
set(CTEST_PARALLEL_LEVEL ${NCORES})
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)
set(CMAKE_CONFIGURATION_TYPES "Debug" "Release" "MinSizeRel" "RelWithDebInfo")

if(NOT DEFAULT_OUTPUT_DIR)
  if(PUBLISH_ARTIFACTS)
    set(DEFAULT_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/publish/${CMAKE_PROJECT_NAME}_${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH}-${GIT_SHA1_TRUNC}")
  else()
    set(DEFAULT_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}-$<CONFIG>")
  endif()
endif()

if(NOT DEFAULT_UTEST_OUTPUT_DIR)
  set(DEFAULT_UTEST_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}-Tests")
endif()

if(APPLE)
  # Compiler id for Apple Clang is now "AppleClang"
  set(CMAKE_POLICY_DEFAULT_CMP0025 NEW)
endif()

# List TARGET_OBJECTS in SOURCES target property
set(CMAKE_POLICY_DEFAULT_CMP0051 NEW)

# INTERPROCEDURAL_OPTIMIZATION is enforced when enabled
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)

# Control generation of Position Independent Executable (PIE)
set(CMAKE_POLICY_DEFAULT_CMP0083 NEW)

# MSVC warning flags are not in CMAKE_<LANG>_FLAGS by default
set(CMAKE_POLICY_DEFAULT_CMP0092 NEW)

# Ninja generators transform DEPFILEs from add_custom_command()
set(CMAKE_POLICY_DEFAULT_CMP0116 NEW)

# MSVC RTTI flag /GR is not added to CMAKE_CXX_FLAGS by default
set(CMAKE_POLICY_DEFAULT_CMP0117 NEW)

# Runtime path (RPATH) entries to add to binaries
list(APPEND CMAKE_BUILD_RPATH "$ORIGIN/.")
list(REMOVE_DUPLICATES CMAKE_BUILD_RPATH)

# C
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_VISIBILITY_PRESET hidden)

# C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# IPO (LTO)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON)

# Miscellaneous
set(CMAKE_OPTIMIZE_DEPENDENCIES ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

# Visual Studio
set(CMAKE_MSVC_RUNTIME_LIBRARY
  "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
)
set(CMAKE_VS_GLOBALS
  "RunCodeAnalysis=false"
  "EnableMicrosoftCodeAnalysis=false"
  "EnableClangTidyCodeAnalysis=false"
)
set(CMAKE_VS_JUST_MY_CODE_DEBUGGING
  "$<IF:$<AND:$<CONFIG:Debug>,$<NOT:$<BOOL:${CLANG_TIDY_CODE_ANALYSIS}>>>,ON,OFF>"
)

# Cppcheck
set(CMAKE_C_CPPCHECK "")
set(CMAKE_CXX_CPPCHECK "")

# Clang-Tidy
set(CMAKE_C_CLANG_TIDY "")
set(CMAKE_CXX_CLANG_TIDY "")

if(MSVC)
  # MSVC exception handling flag /EHsc is not added to CMAKE_CXX_FLAGS by default
  string(REGEX REPLACE "[ \t\r\n]*\/EH[a-z]+" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  string(STRIP "${CMAKE_CXX_FLAGS}" CMAKE_CXX_FLAGS)
endif()

if("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_C_COMPILE_OPTIONS_IPO "-flto=full")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_COMPILE_OPTIONS_IPO "-flto=full")
endif()

if(NOT WIN32)
  set(CMAKE_EXE_LINKER_FLAGS "-m32")
  set(CMAKE_SHARED_LINKER_FLAGS "-m32")

  set(CMAKE_C_FLAGS "-m32 -march=x86-64-v2 -mtune=generic -mmmx -msse -msse2 -msse3 -mssse3 -msse4 -msse4.1 -msse4.2 -mfpmath=sse")
  set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")

  set(CMAKE_C_FLAGS_RELEASE "-O3 -fno-stack-protector -fomit-frame-pointer")
  set(CMAKE_C_FLAGS_MINSIZEREL "-Os -fno-stack-protector -fomit-frame-pointer")

  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    set(CMAKE_C_FLAGS_DEBUG "-O -g")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O1 -g")
  else()
    set(CMAKE_C_FLAGS_DEBUG "-Og -ggdb")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O1 -ggdb")
  endif()

  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
  set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL}")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
endif()

if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
  if(CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL 12)
    set(CMAKE_C_COMPILE_OPTIONS_IPO -flto=auto -fno-fat-lto-objects)
  endif()
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 12)
    set(CMAKE_CXX_COMPILE_OPTIONS_IPO -flto=auto -fno-fat-lto-objects)
  endif()
endif()

set(DEFAULT_ENABLE_RTTI OFF)
set(DEFAULT_ENABLE_EXCEPTIONS OFF)

define_property(TARGET
  PROPERTY
    ENABLE_RTTI

  BRIEF_DOCS
    "Enable RTTI support"

  INITIALIZE_FROM_VARIABLE
    DEFAULT_ENABLE_RTTI
)

define_property(TARGET
  PROPERTY
    ENABLE_EXCEPTIONS

  BRIEF_DOCS
    "Enable exception handling table generation"

  INITIALIZE_FROM_VARIABLE
    DEFAULT_ENABLE_EXCEPTIONS
)
