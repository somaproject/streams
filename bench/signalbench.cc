#include <iostream>
#include <boost/timer.hpp>


#include <fftw3.h>
int main() 
{
  size_t N = 1<< 10;
  fftw_complex *out;
  double * in; 
  fftw_plan p;
  out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (N/2 +1));
  in = (double*) fftw_malloc(sizeof(double) * N);    
  
  boost::timer timer; 
  const int ITERS = 1000000; 
  p = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);
  for (int iter = 0; iter < ITERS; iter++) { 
    for (int i = 0; i < N; i++) {
      in[i] = i * iter; 
    }
    fftw_execute(p); /* repeat as needed */      
  }
  
  fftw_destroy_plan(p);
  double dur = timer.elapsed(); 
  std::cout << dur << " seconds" << std::endl;
  std::cout << ITERS / dur << " blocks per second" << std::endl;
  std::cout << (ITERS*N) / dur << " samples per second" << std::endl;
  
  

  fftw_free(in); fftw_free(out);
}
