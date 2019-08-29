// Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration

// SYCL include(s).
#include <CL/sycl.hpp>

// TBB include(s).
#include <tbb/tbb.h>

// System include(s).
#include <atomic>
#include <iostream>
#include <unistd.h>

/// Type of the queue pool
typedef tbb::concurrent_bounded_queue< cl::sycl::queue* > QueuePool_t;

/// Task testing the parallel performance of SYCL
class SYCLCalcTask {

public:
   /// Constructor
   SYCLCalcTask( QueuePool_t& queuePool,
                 std::atomic_uint& counter )
      : m_queuePool( &queuePool ), m_counter( &counter ) {}

   /// The operator used to execute the task
   void operator()() const {

      // To write a bit fewer characters...
      using namespace cl::sycl::access;

      // Create a large buffer of floating point numbers.
      static const std::size_t BUFFER_SIZE = 1000000;
      cl::sycl::buffer< cl::sycl::cl_float > buffer( BUFFER_SIZE );
      auto bufferWriter = buffer.get_access< mode::write >();
      for( std::size_t i = 0; i < buffer.get_count(); ++i ) {
         bufferWriter[ i ] = 1.5f * i;
      }

      // Get an available queue from the pool.
      cl::sycl::queue* queue;
      m_queuePool->pop( queue );

      // Run a calculation on the buffer using SYCL.
      cl::sycl::range< 1 > workItems( buffer.get_count() );
      auto event = queue->submit( [&]( cl::sycl::handler& handler ) {
            auto accessor =
               buffer.get_access< mode::read_write >( handler );
            handler.parallel_for< class Dummy >( workItems,
                                                 [=]( cl::sycl::id< 1 > id ) {
                                                    for( int i = 0; i < 100;
                                                         ++i ) {
                                                       accessor[ id ] +=
                                                          1.23 * ( accessor[ id ] +
                                                                   2.34 );
                                                    }
                                                 } );
         } );
      event.wait();

      // Put the queue back into the pool.
      m_queuePool->push( queue );

      // Increment the global counter.
      ++( *m_counter );

      return;
   }

private:
   /// Reference to the queue pool used
   QueuePool_t* m_queuePool;
   /// The global counter
   std::atomic_uint* m_counter;

}; // class SYCLCalcTask

int main() {

   // The number of CPU threads to use.
   static const std::size_t CPU_THREADS = 4;
   // The number of calculations to perform
   static const std::size_t N_CALCULATIONS = 100;

   // Set up the SYCL queue pool.
   QueuePool_t queuePool;
   cl::sycl::host_selector deviceSelector;
   for( std::size_t i = 0; i < CPU_THREADS; ++i ) {
      queuePool.push( new cl::sycl::queue( deviceSelector ) );
   }

   // Launch a bunch of calculations.
   std::atomic_uint counter = 0;
   tbb::task_arena arena( CPU_THREADS, 0 );
   for( std::size_t i = 0; i < N_CALCULATIONS; ++i ) {
      arena.enqueue( SYCLCalcTask( queuePool, counter ) );
   }

   // Wait for the calculations to finish.
   while( counter.load() < N_CALCULATIONS ) {
      std::cout << "Processed " << counter.load() << " / "
                << N_CALCULATIONS << " calculations..." << std::endl;
      sleep( 1 );
   }

   // Delete all of the SYCL queues.
   cl::sycl::queue* queue = nullptr;
   while( queuePool.try_pop( queue ) ) {
      delete queue;
   }

   // Return gracefully.
   return 0;
}
