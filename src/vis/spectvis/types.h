#ifndef __SPECTIVIS_TYPES_H__
#define __SPECTIVIS_TYPES_H__

#include <boost/shared_ptr.hpp>
#include <vector> 
#include <elements/types.h>
#include <streamtime.h>

namespace spectvis {

typedef int64_t bufferid_t; 

class FFT {
public:
  timeid_t starttime; 
  timeid_t endtime; 

  std::vector<float> data; 
  
  size_t N; 
  float maxfreq; // final frequency bin 
  
}; 

typedef boost::shared_ptr<FFT> pFFT_t; 

}

#endif
