cmake_minimum_required(VERSION 3.14)
project(fdate VERSION 0.0.1)
include(CPack)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_language(CXX)
enable_language(Fortran)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Dependencies.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PreventInSourceBuilds.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/ProjectOptions.cmake)
fdate_setup_options()
fdate_global_options()
fdate_local_options()
add_library(fdate::fdate_options ALIAS fdate_options)
add_library(fdate::fdate_warnings ALIAS fdate_warnings)
fdate_setup_dependencies()

option(FDATE_ENABLE_FORTRAN "Build Fortran interface" ON)

add_subdirectory(src)

# If the shared build option was not set by a parent project, set it to ON by default
if(NOT DEFINED FDATE_BUILD_SHARED)
  option(FDATE_BUILD_SHARED "Build shared FDATE library" ON)
endif()

option(FDATE_ENABLE_TESTING "Enable FDATE testing" OFF)
if(FDATE_ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

# Configure CPack for packaging
set(CPACK_PACKAGE_NAME "fdate")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VENDOR "Zach Cobell")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

# Add uninstall target
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)

  add_custom_target(
    uninstall COMMAND ${CMAKE_COMMAND} -P
                      ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()
