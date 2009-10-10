#include <iostream>
#include <cassert>
#include <vis/spectvis/fftengine.h>
//#include <somanetwork/logging.h>

using namespace spectvis; 


pFFT_t identity(float * data, int data_size, int N, float fs)
{
  /* 
     Identity operation that just copies the data
     
  */ 
  pFFT_t y(new FFT); 
  y->data.reserve(data_size); 
  for (int i = 0; i < data_size; i++) { 
    y->data.push_back(data[i]); 
  }
  return y; 
}


int main()
{
  //  somanetwork::init_logs(boost::logging::level::fatal); 

  FFTEngine fft(identity); 

}
