cmake_minimum_required(VERSION 3.20)
project(FortranDate)

# Add an option for the user where they can select either 17 or 20
set(FDATE_CXX_STANDARD
    20
    CACHE STRING "C++ standard to use" FORCE)
set_property(CACHE FDATE_CXX_STANDARD PROPERTY STRINGS "17;20")
set(CMAKE_CXX_STANDARD ${FDATE_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_language(CXX)
enable_language(Fortran)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PreventInSourceBuilds.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/ProjectOptions.cmake)
fdate_setup_options()
fdate_global_options()
fdate_local_options()
add_library(fdate::fdate_options ALIAS fdate_options)
add_library(fdate::fdate_warnings ALIAS fdate_warnings)
fdate_setup_dependencies()

add_subdirectory(src)
option(FDATE_BUILD_SHARED "Build shared FDATE library" OFF)
if(FDATE_BUILD_SHARED)
  set(FDATE_LIBRARY_TYPE SHARED)
else()
  set(FDATE_LIBRARY_TYPE STATIC)
endif()

option(FDATE_ENABLE_TESTING "Enable FDATE testing" OFF)
if(FDATE_ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
