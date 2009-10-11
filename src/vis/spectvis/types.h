#ifndef __SPECTIVIS_TYPES_H__
#define __SPECTIVIS_TYPES_H__

#include <boost/shared_ptr.hpp>
#include "boost/multi_array.hpp"

#include <vector> 
#include <elements/types.h>
#include <streamtime.h>

namespace spectvis {

typedef int64_t bufferid_t; 

class FFT {
public:
  timeid_t starttime; // start time of the window this fft is computed over
  timeid_t endtime; // end time of the window this fft is computed over 
  
  size_t overlap; // 

  std::vector<float> data; 
  bufferid_t bufferid; 
  
  size_t N; 
  float maxfreq; // final frequency bin 
  size_t uid; // unique id for lookup in cache
}; 

typedef boost::shared_ptr<FFT> pFFT_t; 

typedef boost::multi_array<float_t,2> spectrogram_t;



class DownsampledFFT {
public:

  timeid_t starttime; // start time of the window this fft is computed over
  timeid_t endtime; // end time of the window this fft is computed over 
  
  size_t overlap; // 

  spectrogram_t data; // should use boost multiarray
  bufferid_t bufferid; 
  
  size_t N; 
  float maxfreq; // final frequency bin 
  size_t uid; // unique id for lookup in cache
}; 

typedef boost::shared_ptr<DownsampledFFT> pDSFFT_t;

}

#endif
