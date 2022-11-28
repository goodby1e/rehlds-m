# setup_target_code_analysis(target1 target2 ...)
function(setup_target_code_analysis)
  #--------------------------------------------------------------------------------------------
  # Include what you use
  #--------------------------------------------------------------------------------------------
  if(IWYU_ANALYSIS)
    find_program(IWYU_PROGRAM
      NAMES "include-what-you-use" "iwyu"
      HINTS "$ENV{PROGRAMFILES}/Include What You Use" "$ENV{PROGRAMFILES}/IWYU"
    )

    if(IWYU_PROGRAM)
      list(APPEND IWYU_PROGRAM
        "-Xiwyu;--cxx17ns"
        "-Xiwyu;--quoted_includes_first"
      )

      foreach(target_name ${ARGV})
        set_target_properties("${target_name}" PROPERTIES
          C_INCLUDE_WHAT_YOU_USE "${IWYU_PROGRAM}"
          CXX_INCLUDE_WHAT_YOU_USE "${IWYU_PROGRAM}"
        )
      endforeach()
    else()
      message(WARNING "IWYU_ANALYSIS option is ON but \"Include what you use\" program is not found.")
    endif()
  endif()

  #--------------------------------------------------------------------------------------------
  # Clang-Tidy
  #--------------------------------------------------------------------------------------------
  if(CLANG_TIDY_CODE_ANALYSIS)
    find_program(CLANG_TIDY_PROGRAM
      NAMES "clang-tidy"
      HINTS "$ENV{PROGRAMFILES}/LLVM/bin"
    )

    if(CLANG_TIDY_PROGRAM)
      foreach(target_name ${ARGV})
        set_target_properties("${target_name}" PROPERTIES
          C_CLANG_TIDY "${CLANG_TIDY_PROGRAM}"
          CXX_CLANG_TIDY "${CLANG_TIDY_PROGRAM}"
        )
      endforeach()
    else()
      message(WARNING "CLANG_TIDY_CODE_ANALYSIS option is ON but Clang-Tidy program is not found.")
    endif()
  endif()

  #--------------------------------------------------------------------------------------------
  # Cppcheck
  #--------------------------------------------------------------------------------------------
  if(CPPCHECK_CODE_ANALYSIS)
    if(WIN32)
      set(PLATFORM_TYPE "win32A")
      find_program(CPPCHECK_PROGRAM NAMES "cppcheck" HINTS "$ENV{PROGRAMFILES}/Cppcheck")
    else()
      set(PLATFORM_TYPE "unix32")
      find_program(CPPCHECK_PROGRAM NAMES "cppcheck")
    endif()

    if(CPPCHECK_PROGRAM)
      list(APPEND CPPCHECK_PROGRAM
        "-j ${NCORES}"
        "--platform=${PLATFORM_TYPE}"
        "--std=c++${CMAKE_CXX_STANDARD}"
        "--cppcheck-build-dir=${CMAKE_BINARY_DIR}"
        "--suppressions-list=${CMAKE_SOURCE_DIR}/.cppcheck_suppressions"
        "--enable=information,missingInclude,performance,portability,style,warning"
        "--library=boost,gnu,googletest,posix,std,windows,zlib"
        "--max-configs=16"
        "--max-ctu-depth=32"
        "--inline-suppr"
        "--quiet"
        "--verbose"
        #"--inconclusive"
      )

      foreach(target_name ${ARGV})
        set(inc_dir "${${target_name}_SOURCE_DIR}/include")
        set(src_dir "${${target_name}_SOURCE_DIR}/src")
        file(MAKE_DIRECTORY ${inc_dir})
        file(MAKE_DIRECTORY ${src_dir})
        list(APPEND CPPCHECK_PROGRAM
          "-I${inc_dir}"
          "-I${src_dir}"
        )
        set_target_properties("${target_name}" PROPERTIES
          C_CPPCHECK "${CPPCHECK_PROGRAM}"
          CXX_CPPCHECK "${CPPCHECK_PROGRAM}"
        )
        list(FILTER CPPCHECK_PROGRAM EXCLUDE REGEX
          "-I${inc_dir}|-I${src_dir}"
        )
      endforeach()
    else()
      message(WARNING "CPPCHECK_CODE_ANALYSIS option is ON but Cppcheck program is not found.")
    endif()
  endif()

  #--------------------------------------------------------------------------------------------
  # PVS-Studio
  #--------------------------------------------------------------------------------------------
  if(PVS_STUDIO_CODE_ANALYSIS)
    FetchContent_MakeAvailable(PVS_CMakeModule)
    include("${pvs_cmakemodule_SOURCE_DIR}/PVS-Studio.cmake")

    if(WIN32)
      set(PLATFORM_TYPE "win32")
    else()
      set(PLATFORM_TYPE "linux32")
    endif()

    foreach(target_name ${ARGV})
      pvs_studio_add_target(
        TARGET "${target_name}.analysis" ALL COMPILE_COMMANDS
        OUTPUT HIDE_HELP MODE "GA:1,2,3+OP:1,2,3"
        PLATFORM "${PLATFORM_TYPE}" CONFIG "${CMAKE_SOURCE_DIR}/.pvs-studio"
        ARGS --threads ${NCORES}
        #CONVERTER_ARGS --excludedCodes V1042
      )
    endforeach()
  endif()
endfunction()
