#include <iostream>
#include "fftwop.h"



namespace spectvis  {

FFTW::FFTW(int N, int MAX_DATA) : 
  N_(N), 
  MAX_DATA_(MAX_DATA)
{
  out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (MAX_DATA)); 
  in = (double*) fftw_malloc(sizeof(double) * MAX_DATA_); 
  
}

pFFT_t FFTW::operator() (float * data, int data_size, int N, float fs) {
//   std::cout << "data_size = " << data_size << " N=" << N << std::endl; 
//  assert(N == N_); 
  for(int i = 0; (i < data_size) && (i < MAX_DATA_); ++i) { 
    in[i] = data[i]; 
  }
  p = fftw_plan_dft_r2c_1d(data_size, in, out, FFTW_ESTIMATE);
  
  fftw_execute(p); 
  
  
  pFFT_t pfft(new FFT); 
  pfft->data.reserve(N); 
  double binwidth = double(data_size /2) / N; 
  double binval = 0.0; 
  double sum = 0.0; 
//   std::cout << "binwidth = " << binwidth << std::endl; 

  for (int i = 0; i < data_size/2; i++) { 
    fftw_complex c; 
    c[0]= out[i][0]; 
    c[1]= out[i][1]; 
    float val = sqrt(c[0]*c[0] + c[1]*c[1]);
    if (binval > binwidth) { 
//       std::cout << "val = " << val << std::endl;
      pfft->data.push_back(sum / binwidth/10.0); 
      sum = val; 
      binval -= binwidth + 1.0; 
    } else {
      sum += val; 
      binval += 1.0; 
    }
  }
  pfft->N = N;  
  pfft->maxfreq = fs/2; 
  
  fftw_destroy_plan(p);
  
  return pfft; 
  }

FFTW::~FFTW()
{
  fftw_free(in); 
  fftw_free(out); 
}


}
