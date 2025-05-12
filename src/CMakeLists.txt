set(FDATE_SOURCES datetime_wrapper.cpp datetime.F90)

# Check for features
include(CheckCXXSourceCompiles)

# std::string_view
check_cxx_source_compiles("#include <string_view>
   int main() { std::string_view s; }" HAS_STRING_VIEW)

add_library(FDATE_interface INTERFACE)
add_library(FDATE_objectlib OBJECT ${FDATE_SOURCES})
add_library(fdate ${FDATE_LIBRARY_TYPE} $<TARGET_OBJECTS:FDATE_objectlib>)

target_compile_definitions(
  FDATE_objectlib PRIVATE $<$<BOOL:${HAS_STRING_VIEW}>:HAS_STRING_VIEW>)
target_compile_definitions(FDATE_objectlib PRIVATE ONLY_C_LOCALE)
set_target_properties(FDATE_objectlib PROPERTIES Fortran_MODULE_DIRECTORY
                                                 ${CMAKE_BINARY_DIR}/mod)

# Set some output properties
set(FDATE_F90_INCLUDE_DIRS
    ${CMAKE_BINARY_DIR}/mod
    CACHE STRING "Include directory for FDATE Fortran modules")
set(FDATE_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    CACHE STRING "Include directory for FDATE C++ headers")
