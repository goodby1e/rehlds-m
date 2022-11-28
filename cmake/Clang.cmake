#--------------------------------------------------------------------------------------------
# Clang compiler options
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
      -Wdouble-promotion
      -Wlogical-op-parentheses

      $<$<COMPILE_LANGUAGE:CXX>:
        -Wcast-qual
        -Wconversion
        -Weffc++
        -Wextra-semi
        -Wfloat-equal
        -Wold-style-cast
        -Wzero-as-null-pointer-constant
      >

      # General
      -fdata-sections                   # Place each data in its own section
      -ffunction-sections               # Place each function in its own section

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

      # AddressSanitizer
      $<$<BOOL:${SANITIZE_ADDRESS}>:
        -fsanitize=address
        -fno-omit-frame-pointer
      >

      # UndefinedBehaviorSanitizer
      $<$<BOOL:${SANITIZE_UNDEFINED}>:
        -fsanitize=undefined
        -fno-omit-frame-pointer
      >
    )
  endforeach()
endfunction()

#--------------------------------------------------------------------------------------------
# Clang link options
#--------------------------------------------------------------------------------------------

# setup_target_link_options(target1 target2 ...)
function(setup_target_link_options)
  if(USE_LINKER_LLD)
    foreach(target_name ${ARGV})
      target_link_options("${target_name}" PRIVATE -fuse-ld=lld
        # Diagnostic flags
        -Wl,--warn-common
        -Wl,--warn-backrefs
        -Wl,--warn-ifunc-textrel
        -Wl,--warn-symbol-ordering

        # General
        -Wl,-O2
        -Wl,--icf=safe
        -Wl,--as-needed
        -Wl,--gc-sections
        -Wl,--no-undefined
        -Wl,--no-lto-legacy-pass-manager
        -Wl,--check-sections

        # Build type RelWithDebInfo
        $<$<CONFIG:RelWithDebInfo>:
          -Wl,--discard-all
          -Wl,--compress-debug-sections=zlib
        >

        # Build type Release, MinSizeRel
        $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:
          -Wl,--lto-O3
          -Wl,--discard-all
          -Wl,--strip-all
        >

        # AddressSanitizer
        $<$<BOOL:${SANITIZE_ADDRESS}>:
          -fsanitize=address
        >

        # UndefinedBehaviorSanitizer
        $<$<BOOL:${SANITIZE_UNDEFINED}>:
          -fsanitize=undefined
        >

        # Print information
        #-Wl,--print-gc-sections
        #-Wl,--print-icf-sections
      )
    endforeach()
  else()
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
  endif()
endfunction()
