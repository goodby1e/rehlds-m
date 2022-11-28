#--------------------------------------------------------------------------------------------
# Common compile definitions
#--------------------------------------------------------------------------------------------

# setup_target_compile_definitions(target1 target2 ...)
function(setup_target_compile_definitions)
  foreach(target_name ${ARGV})
    target_compile_definitions("${target_name}" PRIVATE
      GIT_SHA1=\"${GIT_SHA1}\"
      GIT_SHA1_TRUNC=\"${GIT_SHA1_TRUNC}\"

      # Build type Debug
      $<$<CONFIG:Debug>:
        _DEBUG
      >

      # Build type Release, MinSizeRel, RelWithDebInfo
      $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>,$<CONFIG:RelWithDebInfo>>:
        NDEBUG
      >

      # Boost
      $<$<COMPILE_LANGUAGE:CXX>:
        $<$<NOT:$<BOOL:$<TARGET_PROPERTY:ENABLE_RTTI>>>:
          BOOST_NO_RTTI
          BOOST_NO_TYPEID
        >

        $<$<NOT:$<BOOL:$<TARGET_PROPERTY:ENABLE_EXCEPTIONS>>>:
          BOOST_NO_EXCEPTIONS
          BOOST_EXCEPTION_DISABLE
        >
      >

      # Platform Windows
      $<$<PLATFORM_ID:Windows>:
        # General
        _MBCS
        NOMINMAX

        # Executable
        $<$<STREQUAL:$<UPPER_CASE:$<TARGET_PROPERTY:TYPE>>,EXECUTABLE>:
          _CONSOLE
        >

        # Shared library
        $<$<STREQUAL:$<UPPER_CASE:$<TARGET_PROPERTY:TYPE>>,SHARED_LIBRARY>:
          _USRDLL _WINDLL _WINDOWS
        >

        # Static library
        $<$<STREQUAL:$<UPPER_CASE:$<TARGET_PROPERTY:TYPE>>,STATIC_LIBRARY>:
          _LIB
        >

        # C++
        $<$<COMPILE_LANGUAGE:CXX>:
          $<$<NOT:$<BOOL:$<TARGET_PROPERTY:ENABLE_EXCEPTIONS>>>:
            _HAS_EXCEPTIONS=0
          >
        >
      >

      # Platform Linux, Darwin
      $<$<OR:$<PLATFORM_ID:Linux>,$<PLATFORM_ID:Darwin>>:
        # General
        _LINUX
        LINUX

        # Build type Debug
        $<$<CONFIG:Debug>:
          _FORTIFY_SOURCE=2
          _GLIBCXX_ASSERTIONS=1
          _GLIBCXX_CONCEPT_CHECKS=1
          _GLIBCXX_DEBUG=1
          _GLIBCXX_DEBUG_PEDANTIC=1
        >
      >
    )
  endforeach()
endfunction()
