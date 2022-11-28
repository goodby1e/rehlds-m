#--------------------------------------------------------------------------------------------
# Intel LLVM compiler options
#--------------------------------------------------------------------------------------------

# setup_target_compile_options(target1 target2 ...)
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

      $<$<COMPILE_LANGUAGE:CXX>:
        -Wcast-qual
        -Wconversion
        -Wextra-semi
        -Wfloat-equal
        -Wold-style-cast
        -Wzero-as-null-pointer-constant
      >

      # General
      -pipe                             # Use pipes between commands, when possible
      -fdata-sections                   # Place each data in its own section
      -ffunction-sections               # Place each function in its own section
      -pedantic                         # Warn on language extensions

      # C++
      $<$<COMPILE_LANGUAGE:CXX>:
        -fno-threadsafe-statics         # Thread-safe initialization of local statics

        $<IF:$<BOOL:$<TARGET_PROPERTY:ENABLE_RTTI>>,
          -frtti,-fno-rtti              # Enable/Disable RTTI support
        >

        $<IF:$<BOOL:$<TARGET_PROPERTY:ENABLE_EXCEPTIONS>>,
          -fexceptions,-fno-exceptions  # Enable/Disable C++ exception handling
        >
      >
    )
  endforeach()
endfunction()

#--------------------------------------------------------------------------------------------
# Intel LLVM link options
#--------------------------------------------------------------------------------------------

# setup_target_link_options(target1 target2 ...)
function(setup_target_link_options)
  foreach(target_name ${ARGV})
    target_link_options("${target_name}" PRIVATE
      # Diagnostic flags
      -Wl,--warn-common
      -Wl,--warn-alternate-em

      # General
      -Wl,-O3
      -Wl,--relax
      -Wl,--as-needed
      -Wl,--gc-sections
      -Wl,--no-undefined
      -Wl,--no-allow-shlib-undefined
      -Wl,--check-sections

      # Build type RelWithDebInfo
      $<$<CONFIG:RelWithDebInfo>:
        -Wl,--discard-all
        -Wl,--compress-debug-sections=zlib
      >

      # Build type Release, MinSizeRel
      $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:
        -Wl,--discard-all
        -Wl,--strip-all
      >
    )
  endforeach()
endfunction()
