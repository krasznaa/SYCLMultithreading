# Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration

# Set up the project.
cmake_minimum_required( VERSION 3.13 )
project( BreakSYCL VERSION 1.0.0 LANGUAGES CXX )

# Set up the custom CMake modules of the project.
list( INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake )

# Use C++17 for the build.
set( CMAKE_CXX_STANDARD 17 CACHE STRING
  "The C++ standard to use of the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL
  "Disable the C++ extensions of the compiler" )

# Make sure that both TBB and SYCL are available.
find_package( TBB REQUIRED )
find_package( SYCL REQUIRED )

# Build the executable.
add_executable( sycl_multithreading sycl_multithreading.cxx )
target_include_directories( sycl_multithreading PRIVATE ${TBB_INCLUDE_DIRS} )
target_link_libraries( sycl_multithreading PRIVATE ${TBB_LIBRARIES} )
target_compile_options( sycl_multithreading PRIVATE ${SYCL_FLAGS} )
target_link_options( sycl_multithreading PRIVATE ${SYCL_FLAGS} )
