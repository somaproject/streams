#ifndef __SPECTIVIS_TYPES_H__
#define __SPECTIVIS_TYPES_H__

#include <boost/shared_ptr.hpp>
#include <vector> 
#include <elements/types.h>

namespace spectvis {

typedef elements::timeid_t timeid_t; 

typedef int64_t bufferid_t; 

class FFT {
  timeid_t starttime; 
  timeid_t endtime; 

  std::vector<float> vals; 
  
  size_t N; 
  
}; 

typedef boost::shared_ptr<FFT> pFFT_t; 

}

#endif
