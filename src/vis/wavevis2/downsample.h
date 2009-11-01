#ifndef __VIS_WAVEVIS2_DOWNSAMPLE_H__
#define __VIS_WAVEVIS2_DOWNSAMPLE_H__

#include "types.h"

namespace wavevis2 { 


struct MinMaxVector
{
  /* 
     Note that the times are all relative the star ttime
   */ 
  timeid_t start; 
  timeid_t end; 
  float minval; 
  float maxval; 
  
  std::vector<float> mins; 
  std::vector<float> maxs; 
  std::vector<float> times; 
}; 

typedef boost::shared_ptr<MinMaxVector> pMinMaxVector_t; 

pMinMaxVector_t downsample_minmax_1(pWaveBuffer_t wb); 


}

#endif 
