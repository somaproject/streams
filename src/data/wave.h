#ifndef STREAMS_WAVES_H
#define STREAMS_WAVES_H
#include <boost/shared_ptr.hpp>
#include <vector> 
#include "streamtime.h" 

struct WaveBuffer_t
{
  streamtime_t time; 
  float samprate; 
  std::vector<float> data; 
}; 

typedef boost::shared_ptr<WaveBuffer_t> pWaveBuffer_t; 

#endif // WAVES_H
