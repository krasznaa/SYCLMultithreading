# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# Module used for finding the TBB installation on the build system.
#

# Look for the main TBB header.
find_path( TBB_INCLUDE_DIR
   NAMES "tbb/tbb.h"
   PATH_SUFFIXES "include"
   HINTS ${TBBROOT} ENV TBBROOT
   DOC "Path to the main TBB header file" )
mark_as_advanced( TBB_INCLUDE_DIR )
set( TBB_INCLUDE_DIRS ${TBB_INCLUDE_DIR} )

# Set up which TBB libraries to look for.
set( _components "tbb" ${TBB_FIND_COMPONENTS} )
list( REMOVE_DUPLICATES _components )

# Now look for them.
set( TBB_LIBRARIES )
foreach( _comp ${_components} )

   # Look for this library.
   find_library( TBB_${_comp}_LIBRARY
      NAMES ${_comp}
      PATH_SUFFIXES "lib" "lib64" "lib/intel64/gcc4.8"
      HINTS ${TBBROOT} ENV TBBROOT
      DOC "Path to the ${_comp} TBB library" )
   mark_as_advanced( TBB_${_comp}_LIBRARY )
   list( APPEND TBB_LIBRARIES ${TBB_${_comp}_LIBRARY} )

endforeach()

# Deduce the (interface) version of TBB.
if( TBB_INCLUDE_DIR )
   file( READ "${TBB_INCLUDE_DIR}/tbb/tbb_stddef.h" _tbb_version_file )
   string( REGEX REPLACE ".*#define TBB_VERSION_MAJOR ([0-9]+).*" "\\1"
      TBB_VERSION_MAJOR "${_tbb_version_file}" )
   string( REGEX REPLACE ".*#define TBB_VERSION_MINOR ([0-9]+).*" "\\1"
      TBB_VERSION_MINOR "${_tbb_version_file}" )
   set( TBB_VERSION "${TBB_VERSION_MAJOR}.${TBB_VERSION_MINOR}" )
endif()

# Handle the standard find_package arguments.
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( TBB
   FOUND_VAR TBB_FOUND
   REQUIRED_VARS TBB_INCLUDE_DIR TBB_INCLUDE_DIRS TBB_LIBRARIES
   VERSION_VAR TBB_VERSION )
