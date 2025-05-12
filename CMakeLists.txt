cmake_minimum_required(VERSION 3.20)
project(FortranDate)
set(CMAKE_CXX_STANDARD 17)

enable_language(CXX)
enable_language(Fortran)

add_subdirectory(src)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies.cmake)

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
