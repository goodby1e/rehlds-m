#--------------------------------------------------------------------------------------------
# Common compile definitions
#--------------------------------------------------------------------------------------------

add_compile_definitions(
  # Git
  GIT_SHA1=\"${GIT_SHA1}\"
  GIT_SHA1_TRUNC=\"${GIT_SHA1_TRUNC}\"

  # General
  _LINUX
  LINUX

  # Build type Debug
  $<$<CONFIG:Debug>:_DEBUG>

  # Build type Release, MinSizeRel, RelWithDebInfo
  $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>,$<CONFIG:RelWithDebInfo>>:NDEBUG>

  # Build type Debug
  $<$<CONFIG:Debug>:_FORTIFY_SOURCE=2>
  $<$<CONFIG:Debug>:_GLIBCXX_ASSERTIONS=1>
  $<$<CONFIG:Debug>:_GLIBCXX_CONCEPT_CHECKS=1>
  $<$<CONFIG:Debug>:_GLIBCXX_DEBUG_PEDANTIC=1>
  $<$<CONFIG:Debug>:_GLIBCXX_DEBUG=1>
)

#--------------------------------------------------------------------------------------------
# Common compile options
#--------------------------------------------------------------------------------------------

# C
set(CMAKE_C_FLAGS "")
set(CMAKE_C_FLAGS_DEBUG "")
set(CMAKE_C_FLAGS_RELEASE "")
set(CMAKE_C_FLAGS_MINSIZEREL "")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "")

# C++
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")

add_compile_options(
  -m32                        # Generate code for 32-bit environment
  -march=x86-64-v2            # Generate instructions for a specified machine type
  -mtune=generic              # Tune to the specified cpu-type everything applicable about the generated code
  -mmmx                       # Enable MMX extended instruction set
  -msse                       # Enable SSE extended instruction set
  -msse2                      # Enable SSE2 extended instruction set
  -msse3                      # Enable SSE3 extended instruction set
  -mssse3                     # Enable SSSE3 extended instruction set
  -msse4                      # Enable SSE4 extended instruction set
  -msse4.1                    # Enable SSE4.1 extended instruction set
  -msse4.2                    # Enable SSE4.2 extended instruction set
  -mfpmath=sse                # Use scalar floating-point instructions present in the SSE instruction set
  -pipe                       # Use pipes rather than intermediate files
  -fdata-sections             # Place each data in its own section
  -ffunction-sections         # Place each function in its own section

  # Thread-safe initialization of local statics
  $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>

  # Enable/Disable RTTI support
  $<$<COMPILE_LANGUAGE:CXX>:$<IF:$<BOOL:$<TARGET_PROPERTY:ENABLE_RTTI>>,-frtti,-fno-rtti>>

  # Enable/Disable C++ exception handling
  $<$<COMPILE_LANGUAGE:CXX>:$<IF:$<BOOL:$<TARGET_PROPERTY:ENABLE_EXCEPTIONS>>,-fexceptions,-fno-exceptions>>

  # AddressSanitizer
  $<$<BOOL:${SANITIZE_ADDRESS}>:-fsanitize=address>
  $<$<BOOL:${SANITIZE_ADDRESS}>:-fno-omit-frame-pointer>

  # UndefinedBehaviorSanitizer
  $<$<BOOL:${SANITIZE_UNDEFINED}>:-fsanitize=undefined>
  $<$<BOOL:${SANITIZE_UNDEFINED}>:-fno-omit-frame-pointer>
)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug" AND NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  add_compile_options(
    -Og                       # Optimize debugging experience
    -ggdb                     # Produce debugging information for use by GDB
  )
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  add_compile_options(
    -O3                       # Optimize level 3
    -fno-stack-protector      # Don't emit extra code to check for buffer overflows
    -fomit-frame-pointer      # Don't keep the frame pointer in a register for functions that don't need one
  )
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
  add_compile_options(
    -Os                       # Optimize for size
    -fno-stack-protector      # Don't emit extra code to check for buffer overflows
    -fomit-frame-pointer      # Don't keep the frame pointer in a register for functions that don't need one
  )
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo" AND NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  add_compile_options(
    -O1                       # Optimize level 1
    -ggdb                     # Produce debugging information for use by GDB
  )
endif()

#--------------------------------------------------------------------------------------------
# GCC compile options
#--------------------------------------------------------------------------------------------

if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
  if(CMAKE_C_COMPILER_VERSION VERSION_GREATER_EQUAL 12)
    set(CMAKE_C_COMPILE_OPTIONS_IPO
      -flto=auto              # Run link-time optimizer
      -fno-fat-lto-objects    # Generate a slim link-time optimization (LTO) object
    )
  endif()
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 12)
    set(CMAKE_CXX_COMPILE_OPTIONS_IPO
      -flto=auto              # Run link-time optimizer
      -fno-fat-lto-objects    # Generate a slim link-time optimization (LTO) object
    )
  endif()
endif()

#--------------------------------------------------------------------------------------------
# Clang compile options
#--------------------------------------------------------------------------------------------

if("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_C_COMPILE_OPTIONS_IPO
    -flto=full
  )
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_COMPILE_OPTIONS_IPO
    -flto=full
  )
endif()

#--------------------------------------------------------------------------------------------
# Intel LLVM compile options
#--------------------------------------------------------------------------------------------

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "IntelLLVM")
  if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    add_compile_options(
      -O0                     # Disable all optimizations
      -g                      # Produce debugging information
    )
  endif()

  if("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    add_compile_options(
      -O1                     # Optimize level 1
      -g                      # Produce debugging information
    )
  endif()
endif()

#--------------------------------------------------------------------------------------------
# Common link options
#--------------------------------------------------------------------------------------------

set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_SHARED_LINKER_FLAGS "")

add_link_options(
  -m32                        # Generate code for 32-bit environment

  # Diagnostic flags
  -Wl,--warn-common

  # General
  -Wl,--as-needed
  -Wl,--gc-sections
  -Wl,--no-undefined
)

if("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
  add_link_options(
    -Wl,--discard-all
    -Wl,--compress-debug-sections=zlib
  )
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
  add_link_options(
    -Wl,--discard-all
    -Wl,--strip-all
  )
endif()

#--------------------------------------------------------------------------------------------
# Gold link options
#--------------------------------------------------------------------------------------------

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND USE_LINKER_GOLD)
  add_link_options(
    -fuse-ld=gold             # Use Gold linker

    # Diagnostic flags
    -Wl,--warn-execstack
    -Wl,--warn-drop-version
    -Wl,--warn-shared-textrel
    -Wl,--warn-search-mismatch
    -Wl,--detect-odr-violations
    -Wl,--unresolved-symbols=report-all

    # General
    -Wl,-O3
    -Wl,--relax
    -Wl,--icf=safe
    -Wl,--icf-iterations=5
    -Wl,--no-incremental
    -Wl,--no-whole-archive

    # Print information
    #-Wl,--stats
    #-Wl,--print-gc-sections
    #-Wl,--print-icf-sections
  )

  if("${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    add_link_options(
      -Wl,--no-ld-generated-unwind-info
    )
  endif()

#--------------------------------------------------------------------------------------------
# LLD link options
#--------------------------------------------------------------------------------------------

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" AND USE_LINKER_LLD)
  add_link_options(
    -fuse-ld=lld              # Use LLD linker

    # Diagnostic flags
    -Wl,--warn-backrefs
    -Wl,--warn-ifunc-textrel
    -Wl,--warn-symbol-ordering

    # General
    -Wl,-O2
    -Wl,--icf=safe
    -Wl,--no-lto-legacy-pass-manager
    -Wl,--check-sections

    # AddressSanitizer
    $<$<BOOL:${SANITIZE_ADDRESS}>:-fsanitize=address>

    # UndefinedBehaviorSanitizer
    $<$<BOOL:${SANITIZE_UNDEFINED}>:-fsanitize=undefined>

    # Print information
    #-Wl,--print-gc-sections
    #-Wl,--print-icf-sections
  )

  if("${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    add_link_options(
      -Wl,--lto-O3
    )
  endif()

#--------------------------------------------------------------------------------------------
# LD link options
#--------------------------------------------------------------------------------------------

else()
  add_link_options(
    # Diagnostic flags
    -Wl,--warn-alternate-em

    # General
    -Wl,-O3
    -Wl,--relax
    -Wl,--no-allow-shlib-undefined
    -Wl,--check-sections

    # AddressSanitizer
    $<$<BOOL:${SANITIZE_ADDRESS}>:-fsanitize=address>

    # UndefinedBehaviorSanitizer
    $<$<BOOL:${SANITIZE_UNDEFINED}>:-fsanitize=undefined>
  )
endif()

#--------------------------------------------------------------------------------------------
# Targets compile options
# setup_target_properties(target1 target2 ...)
#--------------------------------------------------------------------------------------------

function(setup_target_properties)
endfunction()

#--------------------------------------------------------------------------------------------
# Targets compile options
# setup_target_compile_options(target1 target2 ...)
#--------------------------------------------------------------------------------------------

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  function(setup_target_compile_options)
    foreach(target_name ${ARGV})
      target_compile_options("${target_name}" PRIVATE
        # Diagnostic flags
        -Wall
        -Wextra
        -Wpedantic
        -Wcast-align
        -Wdouble-promotion
        -Wlogical-op
        -Wnull-dereference
        -Wredundant-decls

        # C++ diagnostic flags
        $<$<COMPILE_LANGUAGE:CXX>:
          -Wcast-qual
          -Wconversion
          -Wctor-dtor-privacy
          -Wduplicated-branches
          -Wduplicated-cond
          -Weffc++
          -Wextra-semi
          -Wfloat-equal
          -Wold-style-cast
          -Woverloaded-virtual
          -Wshadow=compatible-local
          -Wsign-promo
          -Wuseless-cast
          -Wzero-as-null-pointer-constant
        >

        # Static analysis
        $<$<BOOL:${GCC_STATIC_ANALYSIS}>:
          -fanalyzer
        >
      )
    endforeach()
  endfunction()
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  function(setup_target_compile_options)
    foreach(target_name ${ARGV})
      target_compile_options("${target_name}" PRIVATE
        # Diagnostic flags
        -Wall
        -Wextra
        -Wpedantic
        -Wcast-align
        -Wdouble-promotion
        -Wlogical-op-parentheses

        # C++ diagnostic flags
        $<$<COMPILE_LANGUAGE:CXX>:
          -Wcast-qual
          -Wconversion
          -Weffc++
          -Wextra-semi
          -Wfloat-equal
          -Wold-style-cast
          -Wzero-as-null-pointer-constant
        >
      )
    endforeach()
  endfunction()
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "IntelLLVM")
  function(setup_target_compile_options)
    foreach(target_name ${ARGV})
      target_compile_options("${target_name}" PRIVATE
        # Diagnostic flags
        -Wall
        -Wextra
        -Wpedantic
        -Wcast-align
        -Wctor-dtor-privacy
        -Wdeprecated
        -Wdouble-promotion
        -Wlogical-op-parentheses
        -Wnull-dereference
        -Woverloaded-virtual
        -Wredundant-decls
        -Wsign-promo

        # C++ diagnostic flags
        $<$<COMPILE_LANGUAGE:CXX>:
          -Wcast-qual
          -Wconversion
          -Wextra-semi
          -Wfloat-equal
          -Wold-style-cast
          -Wzero-as-null-pointer-constant
        >

        # General
        -pedantic             # Warn on language extensions
      )
    endforeach()
  endfunction()
endif()
