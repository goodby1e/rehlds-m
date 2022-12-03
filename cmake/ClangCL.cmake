#--------------------------------------------------------------------------------------------
# Clang-CL compiler options
#--------------------------------------------------------------------------------------------

# setup_target_compile_options(target1 target2 ...)
function(setup_target_compile_options)
  foreach(target_name ${ARGV})
    target_compile_options("${target_name}" PRIVATE
      /W4                     # Output warning level (/W4 enables -Wall and -Wextra)
      -Wpedantic
      -Wcast-align
      -Wcast-qual
      -Wconversion
      -Wdouble-promotion
      -Weffc++
      -Wextra-semi
      -Wfloat-equal
      -Wlogical-op-parentheses
      -Wold-style-cast
      -Wzero-as-null-pointer-constant

      /diagnostics:caret      # Diagnostics format: prints column and the indicated line of source
      /permissive-            # Standard-conformance mode
      /source-charset:utf-8   # Character set of source files
      /Zc:threadSafeInit-     # Thread-safe local static initialization

      # AddressSanitizer
      $<$<BOOL:${SANITIZE_ADDRESS}>:
        /fsanitize=address
      >

      # C++
      $<$<COMPILE_LANGUAGE:CXX>:
        $<IF:$<BOOL:$<TARGET_PROPERTY:ENABLE_RTTI>>,
          /GR,/GR-            # Enable/Disable RTTI support
        >

        $<$<BOOL:$<TARGET_PROPERTY:ENABLE_EXCEPTIONS>>:
          /EHsc               # Enable C++ exception handling
        >
      >

      # Build type Debug
      $<$<CONFIG:Debug>:
        /GS                   # Checks buffer security
        /Gw-                  # Whole-program global data optimization
        /Gy-                  # Function-level linking
        /Od                   # Disable optimization
        /Oy-                  # Omit frame pointer
        /Zi                   # Generate complete debugging information
      >

      # Build type Release
      $<$<CONFIG:Release>:
        /O2                   # Create fast code
        /Ob2                  # Inline Function expansion
        /Ot                   # Favor fast code
      >

      # Build type MinSizeRel
      $<$<CONFIG:MinSizeRel>:
        /O1                   # Create small code
        /Ob1                  # Inline Function expansion
        /Os                   # Favor small code
      >

      # Build type RelWithDebInfo
      $<$<CONFIG:RelWithDebInfo>:
        /O2                   # Create fast code
        /Ob1                  # Inline Function expansion
        /Ot                   # Favor fast code
      >

      # Build type Release, MinSizeRel, RelWithDebInfo
      $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>,$<CONFIG:RelWithDebInfo>>:
        /GF                   # String pooling
        /GS-                  # Checks buffer security
        /guard:cf-            # Control flow guard security checks
        /guard:ehcont-        # Generate EH Continuation (EHCONT) metadata
        /Gw                   # Whole-program global data optimization
        /Gy                   # Function-level linking
        /Oi                   # Generate intrinsic functions
        /Oy                   # Omit frame pointer
        /Qvec                 # Loop vectorization passes

        $<$<STREQUAL:$<UPPER_CASE:$<TARGET_PROPERTY:TYPE>>,EXECUTABLE>:
          /GA                 # Optimize for Windows applications
        >

        -fcf-protection=none  # Instrument control-flow architecture protection
      >

      # Additional flags
      -march=x86-64-v2 -mmmx -msse -msse2 -msse3 -mssse3 -msse4.1 -msse4.2
    )

    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
      set_target_properties("${target_name}" PROPERTIES
        VS_GLOBAL_UseDebugLibraries true
        VS_GLOBAL_LinkIncremental true
      )
    else()
      set_target_properties("${target_name}" PROPERTIES
        VS_GLOBAL_UseDebugLibraries false
        VS_GLOBAL_LinkIncremental false
      )
    endif()

    if(MICROSOFT_CODE_ANALYSIS OR CLANG_TIDY_CODE_ANALYSIS)
      set_target_properties("${target_name}" PROPERTIES VS_GLOBAL_RunCodeAnalysis true)

      if(MICROSOFT_CODE_ANALYSIS)
        set_target_properties("${target_name}" PROPERTIES VS_GLOBAL_EnableMicrosoftCodeAnalysis true)
      endif()

      if(CLANG_TIDY_CODE_ANALYSIS)
        set_target_properties("${target_name}" PROPERTIES VS_GLOBAL_EnableClangTidyCodeAnalysis true)
      endif()
    endif()
  endforeach()
endfunction()

#--------------------------------------------------------------------------------------------
# Clang-CL link options
#--------------------------------------------------------------------------------------------

# setup_target_link_options(target1 target2 ...)
function(setup_target_link_options)
  foreach(target_name ${ARGV})
    target_link_options("${target_name}" PRIVATE
      /GUARD:NO             # Control flow guard protection
      /MACHINE:X86          # The target platform
      #/MANIFEST:NO         # Create assembly manifest
      /NXCOMPAT             # Compatible with the Windows Data Execution Prevention feature
      /SUBSYSTEM:CONSOLE    # Subsystem
      /WX                   # Treat linker warnings as errors

      # Build type Debug
      $<$<CONFIG:Debug>:
        /DEBUG:FULL         # Debugging information
        /INCREMENTAL        # Incremental linking
      >

      # Build type Release
      $<$<CONFIG:Release>:
        /DEBUG:NONE         # Debugging information
      >

      # Build type MinSizeRel
      $<$<CONFIG:MinSizeRel>:
        /DEBUG:NONE         # Debugging information
      >

      # Build type RelWithDebInfo
      $<$<CONFIG:RelWithDebInfo>:
        /DEBUG              # Debugging information
      >

      # Build type Release, MinSizeRel, RelWithDebInfo
      $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>,$<CONFIG:RelWithDebInfo>>:
        /INCREMENTAL:NO     # Incremental linking
        /LTCG               # Link-time code generation
        /OPT:REF,ICF        # Link optimizations
      >
    )

    # Static libraries linked by default
    target_link_libraries("${target_name}" PRIVATE
      advapi32.lib
      comdlg32.lib
      gdi32.lib
      kernel32.lib
      odbc32.lib
      odbccp32.lib
      ole32.lib
      oleaut32.lib
      shell32.lib
      user32.lib
      uuid.lib
      winspool.lib
    )
  endforeach()
endfunction()
