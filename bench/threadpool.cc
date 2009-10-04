/*! \file
 * \brief Quick start example.
 *
 * This is a very simple example which can be used to configure the threadpool environment on your system. 
 *
 * Copyright (c) 2005-2006 Philipp Henkel
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * http://threadpool.sourceforge.net
 *
 */

#include <iostream>

#include <boost/threadpool.hpp>

using namespace std;
using namespace boost::threadpool;

double x = 0; 

// Some example tasks
void first_task()
{
  x += 1.1; 
}

void second_task()
{
   cout << "second task is running\n" ;
}

int main(int argc,char *argv[])
{
  // Create fifo thread pool container with two threads.
  pool tp(1);
  
  long ITERS = 100000000; 
  // Add some tasks to the pool.
  timeval t1, t2; 
  gettimeofday(&t1, NULL); 
  
  for (long i = 0; i < ITERS; i++) {
    tp.schedule(&first_task);
    //first_task();
  }

  //  Wait until all tasks are finished.
  tp.wait();
  gettimeofday(&t2, NULL); 
  std::cout << "X = " << x << std::endl;
  double elapsed = (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec)/1e6; 
  std::cout << "elapsed = " << elapsed << std::endl;
  std::cout << ITERS / elapsed/1e6 << " M iters/sec" << std::endl; 
   
   // Now all tasks are finished!	
   return(0);
}


