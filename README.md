# Multi-threading Tests With SYCL

This code is here to demonstrate an inefficiency in the current implementation
of [SYCL](https://www.khronos.org/sycl/) provided in
[intel/llvm](https://github.com/intel/llvm), when using it from multiple threads
in parallel.

## Building the Code

To build the code you'll need one of the following two setups:
  - A functional build of Intel's SYCL capable LLVM/Clang compiler from
    [intel/llvm](https://github.com/intel/llvm), and a modern version of
    [TBB](https://github.com/intel/tbb);
  - Any [C++17](https://en.wikipedia.org/wiki/C%2B%2B17) capable C++ compiler,
    and (relatively) modern versions of [TBB](https://github.com/intel/tbb)
    and [Boost](https://www.boost.org/).

In such an environment building the executable should be possible without
any further configuration, you just need to point [CMake](https://cmake.org/)
at the source directory, and build it as any other simple project.

## The Issue

What the code demonstrates is some internal locking in the Intel code that
prevents multiple calculations running on the host through SYCL at the same
time in multiple threads.

Such an issue is not present with [triSYCL](https://github.com/triSYCL/triSYCL).
On the contrary, I'm just not able to convince that implementation to only
use the threads provided by TBB for running... :confused:
