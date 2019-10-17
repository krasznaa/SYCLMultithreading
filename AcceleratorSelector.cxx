// Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration

// Local include(s).
#include "AcceleratorSelector.h"

// System include(s).
#include <string>

int AcceleratorSelector::operator()( const cl::sycl::device& device ) const {

#ifndef TRISYCL_CL_SYCL_HPP
   // Disfavour all NVidia / CUDA devices.
   const std::string vendor =
      device.get_info< cl::sycl::info::device::vendor >();
   if( vendor.find( "NVIDIA" ) != std::string::npos ) {
      return -1;
   }
#endif // not TRISYCL_CL_SYCL_HPP

   // Then, allow any GPUs and other kinds of accelerators to be selected,
   // but not any CPUs, or the host.
   if( device.is_gpu() ) {
      return 500;
   }
   if( device.is_accelerator() ) {
      return 400;
   }
   return -1;
}
