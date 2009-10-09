#include "helper.h"

namespace spectvis
{


std::pair<timeid_t, timeid_t> get_bin_bounds(bufferid_t id, timeid_t windowsize,
					     int overlap)
{
  /* 
     for id, return the start and finish time of the buffer
     
   */ 
  assert(overlap > 0); 
  timeid_t segmentlen = windowsize / overlap; 
  timeid_t starttime = id * segmentlen ; 
  timeid_t endtime = starttime + windowsize -1; 
  return std::make_pair(starttime, endtime); 
    

}

bufferlist_t get_buffers_that_depend_on_times(timeid_t start, timeid_t end, 
					      timeid_t windowsize, int overlap )
{
  
  assert(overlap > 0); 

  timeid_t segmentlen = windowsize / overlap; 
  
  bufferlist_t output; 
  timeid_t lowerbound = start - windowsize; 
  int lowcnt = lowerbound / segmentlen - 1; 
  
  timeid_t upperbound = end + windowsize; 
  int uppercnt = upperbound / segmentlen + 1; 
  for (int i = lowcnt; i < uppercnt; ++i) { 

    std::pair<timeid_t, timeid_t> val = get_bin_bounds(i, windowsize, overlap); 
    
    if (((val.first >= start) and   (val.first <= end)) or
	((val.second >= start) and   (val.second <= end))) {
      output.push_back(i); 
    }
  }
  
  return output;
}


}
