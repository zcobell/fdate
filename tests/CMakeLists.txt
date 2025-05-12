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

  if(CDATE_ENABLE_COVERAGE)
    set_target_properties(
      ${test_name} PROPERTIES COMPILE_FLAGS ${CDATE_COVERAGE_COMPILE_FLAGS}
                              LINK_FLAGS ${CDATE_COVERAGE_LINK_FLAGS})
  endif()

  target_link_libraries(${test_name} PRIVATE Catch2::Catch2WithMain)
  set_target_properties(
    ${test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                            ${CMAKE_CURRENT_BINARY_DIR}/tests)
  add_dependencies(${test_name} Catch2::Catch2WithMain)
endforeach()
# ##############################################################################

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

  target_link_libraries(${test_name} PRIVATE fdate)
  set_target_properties(
    ${test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                            ${CMAKE_CURRENT_BINARY_DIR}/tests)
  add_dependencies(${test_name} fdate)
endforeach()
# ##############################################################################
