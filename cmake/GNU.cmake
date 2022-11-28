#--------------------------------------------------------------------------------------------
# GCC compiler options
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
      -Wlogical-op
      -Wnull-dereference
      -Wredundant-decls

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

      # General
      -pipe                             # Use pipes rather than intermediate files
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

      # Static analysis
      $<$<BOOL:${GCC_STATIC_ANALYSIS}>:
        -fanalyzer                      # Enable static analysis pass
      >
    )
  endforeach()
endfunction()

#--------------------------------------------------------------------------------------------
# GCC link options
#--------------------------------------------------------------------------------------------

# setup_target_link_options(target1 target2 ...)
function(setup_target_link_options)
  if(USE_LINKER_GOLD)
    foreach(target_name ${ARGV})
      target_link_options("${target_name}" PRIVATE -fuse-ld=gold
        # Diagnostic flags
        -Wl,--warn-common
        -Wl,--warn-execstack
        -Wl,--warn-drop-version
        -Wl,--warn-shared-textrel
        -Wl,--warn-search-mismatch
        -Wl,--detect-odr-violations
        -Wl,--unresolved-symbols=report-all

        # General
        -Wl,-O3
        -Wl,--relax
        -Wl,--as-needed
        -Wl,--gc-sections
        -Wl,--icf=safe
        -Wl,--icf-iterations=5
        -Wl,--no-undefined
        -Wl,--no-incremental
        -Wl,--no-whole-archive

        # Build type RelWithDebInfo
        $<$<CONFIG:RelWithDebInfo>:
          -Wl,--discard-all
          -Wl,--compress-debug-sections=zlib
        >

        # Build type Release, MinSizeRel
        $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>>:
          -Wl,--no-ld-generated-unwind-info
          -Wl,--discard-all
          -Wl,--strip-all
        >

        # Print information
        #-Wl,--stats
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

        # AddressSanitizer
        $<$<BOOL:${SANITIZE_ADDRESS}>:
          -fsanitize=address
        >

        # UndefinedBehaviorSanitizer
        $<$<BOOL:${SANITIZE_UNDEFINED}>:
          -fsanitize=undefined
        >
      )
    endforeach()
  endif()
endfunction()
