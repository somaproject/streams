#include <iostream>
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
  /* 
     Here's a time range -- which buffers contain these times? 

   */ 
//   std::cout << "start = " << start << " end =" << end 
// 	    << " windowsize =" << windowsize << " overlap = " << overlap
// 	    << std::endl; 

  assert(overlap > 0); 

  timeid_t segmentlen = windowsize / overlap; 
//   std::cout << "segmentlen = " << segmentlen; 
  
  bufferlist_t output; 
  timeid_t lowerbound = start - windowsize; 
  int lowcnt = lowerbound / segmentlen - 1; 
  
//   std::cout << " lowcnt = " << lowcnt << " lowerbound =" << lowerbound; 
  
  timeid_t upperbound = end + windowsize; 
  int uppercnt = upperbound / segmentlen + 1; 
//   std::cout << " uperbound = " << upperbound
// 	    << " uppercount =" <<  uppercnt << std::endl; 
  for (int i = lowcnt; i < uppercnt; ++i) { 

    std::pair<timeid_t, timeid_t> val = get_bin_bounds(i, windowsize, overlap); 
    timeid_t bin_start = val.first; 
    timeid_t bin_end = val.second; 
    if (((bin_start >= start) and   (bin_start <= end)) or
	((bin_end >= start) and   (bin_end <= end))) {
      output.push_back(i); 
    } else { 
      // check endpoints
      if ((start >= bin_start and start <= bin_end) or
	  (end >= bin_start and end <= bin_end)) {
	output.push_back(i);

      }
      
    }
  }
  
  return output;
}


}
