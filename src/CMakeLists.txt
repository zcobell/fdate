include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/SystemLink.cmake)

set(FDATE_SOURCES datetime_wrapper.cpp)

if(FDATE_ENABLE_FORTRAN)
  set(FDATE_SOURCES ${FDATE_SOURCES} datetime.F90)
endif()

# Check for features
include(CheckCXXSourceCompiles)

# std::string_view
check_cxx_source_compiles("#include <string_view>
   int main() { std::string_view s; }" HAS_STRING_VIEW)

add_library(fdate_interface INTERFACE)
add_library(fdate_objectlib OBJECT ${FDATE_SOURCES})
add_library(fdate ${FDATE_LIBRARY_TYPE} $<TARGET_OBJECTS:fdate_objectlib>)

target_link_libraries(fdate_objectlib PUBLIC fdate::fdate_warnings
                                             fdate::fdate_options)

target_compile_definitions(
  fdate_objectlib PRIVATE $<$<BOOL:${HAS_STRING_VIEW}>:HAS_STRING_VIEW>)
target_compile_definitions(fdate_objectlib PRIVATE ONLY_C_LOCALE)
set_target_properties(fdate_objectlib PROPERTIES Fortran_MODULE_DIRECTORY
                                                 ${CMAKE_BINARY_DIR}/mod)

target_include_directories(
  fdate_objectlib SYSTEM
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty/date_hh)

# Set some output properties
set(fdate_F90_INCLUDE_DIRS
    ${CMAKE_BINARY_DIR}/mod
    CACHE STRING "Include directory for FDATE Fortran modules")
set(fdate_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    CACHE STRING "Include directory for FDATE C++ headers")
