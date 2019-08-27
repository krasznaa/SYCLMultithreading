# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#
# This module is used to find out whether the C++ compiler set up to build the
# project is able to compile SYCL code. If so, it provides helper variables
# to the project configuration to set up the build of SYCL
# libraries/executables.
#

# Reaffirm that for this piece of code we need at least CMake 3.13.
cmake_minimum_required( VERSION 3.13 )

# Greet the user.
if( NOT SYCL_FIND_QUIETLY )
   message( STATUS "Checking if ${CMAKE_CXX_COMPILER} is SYCL capable..." )
endif()

# If the compiler is not Clang, then don't even try anything more. Since for
# now I'm only seriously experimenting with the Intel LLVM/Clang code.
if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )

   # Check if it's possible to use the <CL/sycl.hpp> header using the version
   # of Clang given to CMake. Note that this check will use the OpenCL library
   # from the system. Which is the one that Clang-sycl is always compiled
   # against on my machines. But this code could be made smarter later on...
   include( CheckIncludeFileCXX )
   set( CMAKE_REQUIRED_FLAGS -fsycl )
   set( CMAKE_REQUIRED_LIBRARIES OpenCL )
   check_include_file_cxx( "CL/sycl.hpp" SYCL_FOUND )

endif()

# Set up the variables that the project should use.
if( SYCL_FOUND )
   if( NOT SYCL_FIND_QUIETLY )
      message( STATUS
         "Checking if ${CMAKE_CXX_COMPILER} is SYCL capable... success" )
   endif()
   set( SYCL_FLAGS "-fsycl" )
else()
   if( NOT SYCL_FIND_QUIETLY )
      message( STATUS
         "Checking if ${CMAKE_CXX_COMPILER} is SYCL capable... failure" )
   endif()
endif()
