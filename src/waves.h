#ifndef WAVES_H
#define WAVES_H
#include <boost/shared_ptr.hpp>


struct WaveBuffer_t
{
  float time; 
  float samprate; 
  std::vector<float> data; 
}; 

typedef boost::shared_ptr<WaveBuffer_t> pWaveBuffer_t; 


#endif // WAVES_H
