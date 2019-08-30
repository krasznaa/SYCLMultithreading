// Dear emacs, this is -*- c++ -*-
// Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
#ifndef SYCLMULTITHREADING_ACCELERATORSELECTOR_H
#define SYCLMULTITHREADING_ACCELERATORSELECTOR_H

// SYCL include(s).
#include <CL/sycl.hpp>

/// Accelerator device selector
///
/// This selector is used to select "viable" accelerators in my tests,
/// specifically on the devices that I have available.
///
/// @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
///
class AcceleratorSelector : public cl::sycl::device_selector {

public:
   /// @name Function(s) inherited from @c cl::sycl::device_selector
   /// @{

   /// Function used for scoring the different devices
   virtual int operator()( const cl::sycl::device& device ) const override;

   /// @}

}; // class AcceleratorSelector

#endif // SYCLMULTITHREADING_ACCELERATORSELECTOR_H
