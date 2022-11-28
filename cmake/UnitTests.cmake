# NAME        Test name
# SOURCES     Sources to use when building
# LIBRARIES   Libraries to use when linking
# OUTPUT_DIR  Output directory in which to build target files
function(setup_unit_tests)
  cmake_parse_arguments(
    "UTEST"
    ""
    "NAME;OUTPUT_DIR"
    "SOURCES;LIBRARIES"
    ${ARGN}
  )

  if(NOT UTEST_NAME)
    if(ARGV0)
      set(UTEST_NAME "${ARGV0}")
    else()
      message(FATAL_ERROR "NAME is not set.")
    endif()
  endif()

  if(NOT UTEST_SOURCES)
    message(FATAL_ERROR "SOURCES is not set.")
  endif()

  if(NOT UTEST_OUTPUT_DIR)
    set(UTEST_OUTPUT_DIR "${DEFAULT_UTEST_OUTPUT_DIR}")
  endif()

  if(BUILD_UNIT_TESTS)
    FetchContent_MakeAvailable(GoogleTest)
    add_executable("${UTEST_NAME}" ${UTEST_SOURCES})

    set_target_properties("${UTEST_NAME}" gmock gmock_main gtest gtest_main PROPERTIES
      ENABLE_RTTI ON
      ENABLE_EXCEPTIONS ON
      MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
      RUNTIME_OUTPUT_DIRECTORY "${UTEST_OUTPUT_DIR}"
      LIBRARY_OUTPUT_DIRECTORY "${UTEST_OUTPUT_DIR}"
      COMPILE_PDB_OUTPUT_DIRECTORY_DEBUG "${UTEST_OUTPUT_DIR}"
      COMPILE_PDB_OUTPUT_DIRECTORY_RELWITHDEBINFO "${UTEST_OUTPUT_DIR}"
    )

    target_link_libraries("${UTEST_NAME}" PRIVATE
      GTest::gtest_main
      ${UTEST_LIBRARIES}
    )

    # Disable all warnings for GoogleTest targets
    target_compile_options(gmock PRIVATE $<IF:$<PLATFORM_ID:Windows>,/w,-w>)
    target_compile_options(gmock_main PRIVATE $<IF:$<PLATFORM_ID:Windows>,/w,-w>)
    target_compile_options(gtest PRIVATE $<IF:$<PLATFORM_ID:Windows>,/w,-w>)
    target_compile_options(gtest_main PRIVATE $<IF:$<PLATFORM_ID:Windows>,/w,-w>)

    setup_target_compile_definitions("${UTEST_NAME}")
    setup_target_compile_options("${UTEST_NAME}")
    setup_target_link_options("${UTEST_NAME}")

    add_test(
      NAME "${UTEST_NAME}"
      COMMAND "$<TARGET_FILE:${UTEST_NAME}>"
    )

    if(UNIX AND NOT APPLE AND NOT NO_INTEL_LIB AND NOT LINK_STATIC_INTEL)
      add_custom_command(
        TARGET "${UTEST_NAME}" POST_BUILD
        WORKING_DIRECTORY "$<TARGET_FILE_DIR:${UTEST_NAME}>"
        COMMAND sh -c "cp \$(ldd ${UTEST_NAME} | grep 'libintlc.so' | grep -E -o '/.+\\s') . > /dev/null 2>&1 || (exit 0)"
        VERBATIM
      )
    endif()
  endif()
endfunction()
