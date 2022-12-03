#--------------------------------------------------------------------------------------------
# MSVC compiler options
#--------------------------------------------------------------------------------------------

# setup_target_compile_options(target1 target2 ...)
function(setup_target_compile_options)
  foreach(target_name ${ARGV})
    target_compile_options("${target_name}" PRIVATE
      /W4                     # Output warning level
      /wd4706                 # Disable warning (assignment within conditional expression)

      /arch:SSE2              # Minimum CPU architecture requirements
      /cgthreads${NCORES}     # Number of cl.exe threads to use for optimization and code generation
      /diagnostics:caret      # Diagnostics format: prints column and the indicated line of source
      /external:anglebrackets # Treat all headers included via <> as external
      /external:templates-    # Evaluate warning level across template instantiation chain
      /external:W0            # Warning level for external headers
      /FC                     # Displays the full path of source code files passed to cl.exe in diagnostic text
      /MP${NCORES}            # Build with multiple processes
      /options:strict         # Unrecognized compiler options are errors
      /permissive-            # Standard-conformance mode
      /source-charset:utf-8   # Character set of source files
      /validate-charset       # Validate UTF-8 files for only compatible characters
      /Zc:threadSafeInit-     # Thread-safe local static initialization

      $<$<OR:$<BOOL:${MICROSOFT_CODE_ANALYSIS}>,$<BOOL:${CLANG_TIDY_CODE_ANALYSIS}>>:
        /analyze              # Enable code analysis
        /analyze:external-    # Skip analysis of external header files
      >

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
        /JMC                  # Just My Code debugging
        /Od                   # Disable optimization
        /Oy-                  # Omit frame pointer
        /RTC1                 # Runtime checks
        /sdl                  # Enable more security features and warnings
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
        /Zo-                  # Generate richer debugging information for optimized code
      >

      # Build type Release, MinSizeRel, RelWithDebInfo
      $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>,$<CONFIG:RelWithDebInfo>>:
        /GF                   # String pooling
        /GL                   # Whole program optimization
        /GS-                  # Checks buffer security
        /guard:cf-            # Control flow guard security checks
        /guard:ehcont-        # Generate EH Continuation (EHCONT) metadata
        /Gw                   # Whole-program global data optimization
        /Gy                   # Function-level linking
        /JMC-                 # Just My Code debugging
        /Oi                   # Generate intrinsic functions
        /Oy                   # Omit frame pointer
        /sdl-                 # Enable more security features and warnings
        /Zc:inline            # Remove unreferenced functions or data if they're COMDAT or have internal linkage only

        $<$<NOT:$<BOOL:${SANITIZE_ADDRESS}>>:
          /Qpar               # Automatic parallelization of loops
        >

        $<$<STREQUAL:$<UPPER_CASE:$<TARGET_PROPERTY:TYPE>>,EXECUTABLE>:
          /GA                 # Optimize for Windows applications
        >
      >
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
# MSVC link options
#--------------------------------------------------------------------------------------------

# setup_target_link_options(target1 target2 ...)
function(setup_target_link_options)
  foreach(target_name ${ARGV})
    target_link_options("${target_name}" PRIVATE
      /CGTHREADS:${NCORES}  # Number of cl.exe threads to use for optimization and code generation
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
        /DEBUG:FASTLINK     # Debugging information
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
