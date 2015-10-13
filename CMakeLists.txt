#
#
# top level build file for the SPUDlib

## prepare CMAKE
cmake_minimum_required ( VERSION 3.2.0 )

project ( "sltest" VERSION "1.0.0")

## setup options
option ( verbose "Produce verbose makefile output" OFF )

set ( dist_dir    ${CMAKE_BINARY_DIR}/dist )
set ( prefix      ${CMAKE_INSTALL_PREFIX} )
set ( exec_prefix ${CMAKE_INSTALL_PREFIX}/bin )
set ( libdir      ${CMAKE_INSTALL_PREFIX}/lib )
set ( includedir  ${CMAKE_INSTALL_PREFIX}/include )

set ( package_prefix "${CMAKE_PACKAGE_NAME}-${CMAKE_SYSTEM_NAME}" )

set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${dist_dir}/bin )
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${dist_dir}/lib )
set ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${dist_dir}/lib )
set ( CMAKE_BUILD_TYPE Debug )

## check and generate configuration

include ( CheckIncludeFiles )
include ( CheckLibraryExists )
include ( CheckFunctionExists )
include ( CheckTypeSize )

check_library_exists ( pthread pthread_create "" HAVE_LIBPTHREAD )
check_library_exists ( m tan "" HAVE_LIBM )

## setup global compiler options
include_directories ( ${CMAKE_CURRENT_BINARY_DIR} )
if ( CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
     CMAKE_C_COMPILER_ID MATCHES "Clang" )
  message ( STATUS "adding GCC/Clang options ")
  add_definitions ( -std=gnu99 -Wall -Wextra -pedantic )
  ## disable VLA "is a GNU extension" warning
  add_definitions ( -Wno-gnu-zero-variadic-macro-arguments )
  if ( fatal_warnings )
    add_definitions ( -Werror )
  endif ()
  if ( optimize )
    add_definitions ( -O2 )
  endif ()
elseif ( MSVC )
  add_definitions ( /W3 )
  if ( fatal_warnings )
    add_definitions ( /WX )
  endif ()
else ()
  message ( FATAL_ERROR "unhandled compiler id: ${CMAKE_C_COMPILER_ID}" )
endif ()
if ( verbose )
  set ( CMAKE_VERBOSE_MAKEFILE ON )
endif ()

Include(ExternalProject)
ExternalProject_Add(
   project_cn-cbor
   GIT_REPOSITORY https://github.com/hildjj/cn-cbor.git
   CMAKE_ARGS -Doptimize=OFF -Duse_context=ON -Dbuild_docs=OFF -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
   INSTALL_DIR "${dist_dir}"
   UPDATE_DISCONNECTED 1
)

ExternalProject_Get_Property(project_cn-cbor install_dir)
include_directories ( "${install_dir}/include" )
add_library(cn-cbor STATIC IMPORTED)
set_property(TARGET cn-cbor PROPERTY IMPORTED_LOCATION "${install_dir}/lib/${CMAKE_SHARED_MODULE_PREFIX}cn-cbor${CMAKE_SHARED_LIBRARY_SUFFIX}")
add_dependencies(cn-cbor project_cn-cbor)


ExternalProject_Add(
   project_spud
   GIT_REPOSITORY https://github.com/iptube/SPUDlib.git
   CMAKE_ARGS -Doptimize=OFF -Dbuild_docs=OFF -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
   INSTALL_DIR "${dist_dir}"
   UPDATE_DISCONNECTED 1
)

ExternalProject_Get_Property(project_spud install_dir)
include_directories ( "${install_dir}/include" )
add_library(spud STATIC IMPORTED)
set_property(TARGET spud PROPERTY IMPORTED_LOCATION "${install_dir}/lib/${CMAKE_SHARED_MODULE_PREFIX}spud${CMAKE_SHARED_LIBRARY_SUFFIX}")
add_dependencies(spud project_spud)

## include the parts
add_subdirectory ( src )
