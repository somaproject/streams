#ifndef STREAMS_WAVES_H
#define STREAMS_WAVES_H
#include <boost/shared_ptr.hpp>
#include <vector> 
#include <boost/array.hpp>

#include "streamtime.h" 

struct WaveBuffer_t
{
  streamtime_t time; 
  float samprate; 
  std::vector<float> data; 
}; 

typedef boost::shared_ptr<WaveBuffer_t> pWaveBuffer_t; 

template<int N> 
struct WaveBufferDisk_t
{
  streamtime_t time; 
  float samprate; 
  boost::array<float, N> data;   
  
  void copy(WaveBuffer_t * wb) {
    wb->time = time; 
    wb->samprate = samprate; 
    wb->data.resize(N); 
    for (int i = 0; i < N; i++) {
      wb->data[i] = data[i]; 
    }

  }
}; 

#endif // WAVES_H
