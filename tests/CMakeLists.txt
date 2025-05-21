# ##############################################################################
# ...CXX TESTS
# ##############################################################################
file(GLOB_RECURSE TestList ${CMAKE_CURRENT_SOURCE_DIR}/cxx/TEST_*.cpp)

foreach(test ${TestList})
  get_filename_component(test_name ${test} NAME_WE)
  add_executable(${test_name} ${test})
  add_test(
    NAME ${test_name}
    COMMAND ${test_name}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

  target_include_directories(
    ${test_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src
                         ${catch2_SOURCE_DIR})

  target_link_libraries(${test_name} PRIVATE fdate::fdate_warnings
                                             fdate::fdate_options)

  if(FDATE_ENABLE_COVERAGE)
    set_target_properties(
      ${test_name} PROPERTIES COMPILE_FLAGS ${FDATE_COVERAGE_COMPILE_FLAGS}
                              LINK_FLAGS ${FDATE_COVERAGE_LINK_FLAGS})
  endif()

  target_include_directories(
    ${test_name} SYSTEM
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/date_hh)

  target_link_libraries(${test_name} PRIVATE Catch2::Catch2WithMain)
  set_target_properties(
    ${test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                            ${CMAKE_CURRENT_BINARY_DIR}/tests)
  add_dependencies(${test_name} Catch2::Catch2WithMain)
endforeach()
# ##############################################################################

# ##############################################################################
# Check CXX results against Python results
# ##############################################################################
find_package(Python3 COMPONENTS Interpreter)
if(Python3_FOUND)
  add_executable(datetime_interface
                 ${CMAKE_CURRENT_SOURCE_DIR}/cxx/cxx_test_interface.cpp)
  target_include_directories(datetime_interface
                             PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)
  target_include_directories(
    datetime_interface SYSTEM
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/date_hh)

  target_link_libraries(${test_name} PRIVATE fdate fdate::fdate_warnings
                                             fdate::fdate_options)

  # Add a test which calls the Python script that runs random tests against
  # python's datetime library
  add_test(
    NAME TEST_python_fuzz_parse
    COMMAND
      ${Python3_EXECUTABLE}
      ${CMAKE_CURRENT_SOURCE_DIR}/cxx/cxx_python_fuzz_wrapper.py --test-count
      1000 --test parse --exe ${CMAKE_BINARY_DIR}/tests/datetime_interface
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

  add_test(
    NAME TEST_python_fuzz_arithmetic
    COMMAND
      ${Python3_EXECUTABLE}
      ${CMAKE_CURRENT_SOURCE_DIR}/cxx/cxx_python_fuzz_wrapper.py --test-count
      1000 --test arithmetic --exe ${CMAKE_BINARY_DIR}/tests/datetime_interface
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# ##############################################################################
# ... Fortran TESTS
# ##############################################################################
file(GLOB_RECURSE TestList ${CMAKE_CURRENT_SOURCE_DIR}/fortran/TEST_ftn_*.F90)
foreach(test ${TestList})
  get_filename_component(test_name ${test} NAME_WE)
  add_executable(${test_name} ${test})
  add_test(
    NAME ${test_name}
    COMMAND ${test_name}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  target_include_directories(${test_name} PRIVATE ${CMAKE_BINARY_DIR}/mod)

  target_link_libraries(${test_name} PRIVATE fdate fdate::fdate_warnings
                                             fdate::fdate_options)

  set_target_properties(
    ${test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                            ${CMAKE_CURRENT_BINARY_DIR}/tests)
  add_dependencies(${test_name} fdate)
endforeach()
# ##############################################################################

# Now conditionally add fuzzing tests when using Clang
if(FDATE_ENABLE_FUZZ_TEST)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

    # Add the fuzzer executable
    add_executable(datetime_fuzzer fuzz/TEST_cxx_fuzz.cpp)

    # Add fuzzing compilation flags
    target_compile_options(
      datetime_fuzzer PRIVATE -g -O1 -fsanitize=fuzzer,address,undefined
                              -fno-omit-frame-pointer)

    # Add fuzzing link flags
    target_link_options(datetime_fuzzer PRIVATE
                        -fsanitize=fuzzer,address,undefined)

    # Add deterministic tests with internal seeds The fuzzer will use the
    # built-in test cases with no arguments
    add_test(NAME FuzzTest_Deterministic COMMAND $<TARGET_FILE:datetime_fuzzer>
                                                 -runs=10)

    # Add a short fuzzing run
    add_test(NAME FuzzTest_ShortFuzz COMMAND $<TARGET_FILE:datetime_fuzzer>
                                             -max_len=4096 -max_total_time=10)
    set_tests_properties(FuzzTest_ShortFuzz PROPERTIES TIMEOUT 15)

    # Add the include directories for each test
    target_include_directories(
      datetime_fuzzer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src
                              ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/date_hh)
  endif()
endif()
