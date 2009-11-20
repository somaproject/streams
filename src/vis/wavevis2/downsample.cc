#include <boost/foreach.hpp>
#include "downsample.h"

namespace wavevis2 {

pMinMaxVector_t downsample_minmax_1(pWaveBuffer_t wb)
{
  pMinMaxVector_t mmv(new MinMaxVector); 
  mmv->start = wb->time; 
  mmv->end = wb->time + timeid_t(1.0/double(wb->samprate) *wb->data.size() * 1e9);

  mmv->minval = std::numeric_limits<float>::max(); 
  mmv->maxval = -std::numeric_limits<float>::max(); 
  BOOST_FOREACH(float x, wb->data) {
    if(mmv->minval > x) { 
      mmv->minval = x; 
    }

    if (mmv->maxval < x) {
      mmv->maxval = x; 
    }
    
  }

  float time = 0; 
  float min = std::numeric_limits<float>::max(); 
  float max = -std::numeric_limits<float>::max(); 

  int ds = 20; 
  for(int i = 0; i < wb->data.size(); i++) { 
    if (min > wb->data[i]) { 
      min = wb->data[i]; 
    } 

    if (max < wb->data[i] ) { 
      max = wb->data[i];
    }

    if((i % ds == 0) or (i == wb->data.size()-1)) { 
      mmv->mins.push_back(min); 
      mmv->maxs.push_back(max); 
      mmv->times.push_back(1.0/double(wb->samprate) * i); 
      min = std::numeric_limits<float>::max(); 
      max = -std::numeric_limits<float>::max(); 
    }
  }
  if (mmv->times.size() > 1) {
    mmv->mins[0] = mmv->mins[1];
    mmv->maxs[0] = mmv->maxs[1]; 

  }
  
  return mmv; 

}


pMinMaxVector_t downsample_minmax_var(pWaveBuffer_t wb, int scale)
{
  pMinMaxVector_t mmv(new MinMaxVector); 
  mmv->start = wb->time; 
  mmv->end = wb->time + timeid_t(1.0/double(wb->samprate) *wb->data.size() * 1e9);

  mmv->minval = std::numeric_limits<float>::max(); 
  mmv->maxval = -std::numeric_limits<float>::max(); 
  BOOST_FOREACH(float x, wb->data) {
    if(mmv->minval > x) { 
      mmv->minval = x; 
    }

    if (mmv->maxval < x) {
      mmv->maxval = x; 
    }
    
  }

  float time = 0; 
  float min = std::numeric_limits<float>::max(); 
  float max = -std::numeric_limits<float>::max(); 

  int ds = scale; 
  for(int i = 0; i < wb->data.size(); i++) { 
    if (min > wb->data[i]) { 
      min = wb->data[i]; 
    } 

    if (max < wb->data[i] ) { 
      max = wb->data[i];
    }

    if((i % ds == 0) or (i == wb->data.size()-1)) { 

      mmv->mins.push_back(min); 
      mmv->maxs.push_back(max); 
      mmv->times.push_back(1.0/double(wb->samprate) * i); 
      min = std::numeric_limits<float>::max(); 
      max = -std::numeric_limits<float>::max(); 
    }
  }
  if (mmv->times.size() > 1) {
    mmv->mins[0] = mmv->mins[1];
    mmv->maxs[0] = mmv->maxs[1]; 

  }
  
  return mmv; 

}



}
