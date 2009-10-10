#ifndef __SPECTVIS_HELPER_H__
#define __SPECTVIS_HELPER_H__

#include <list>
#include <map>

#include "types.h"

namespace spectvis {
  
typedef std::list<bufferid_t> bufferlist_t; 

std::list<int64_t> bin_range(timeid_t t1, timeid_t t2, timeid_t binsize); 

bufferlist_t get_buffers_that_depend_on_times(timeid_t start, timeid_t end, 
					      timeid_t windowsize, int overlap); 

std::pair<timeid_t, timeid_t> get_bin_bounds(bufferid_t id, timeid_t windowsize,
					     int overlap);

template<typename T> 
class RangeQuery
{
  /* 
     Query object supporting range (interval) queries based on timeid_t; 
     
     We assume the ranges are 1. non-overlapping and. 2. don't have
     the same start time. 

  */
public:
  RangeQuery() {

  }
  
  void reset() { 
    ranges_.clear(); 
    
  }
  
  void insert(T val, timeid_t start, timeid_t end) {
    Range r; 
    r.val = val; 
    r.start = start; 
    r.end = end; 
    ranges_.insert(std::make_pair(start, r)); 
    
  }

  std::list<T> query(timeid_t start, timeid_t end) { 
    /* Return a list of all elements that have some
       part of their interval between start and end

    */ 
    std::list<T> results; 
    
    typename rangemap_t::iterator i = ranges_.lower_bound(start);
    if (i != ranges_.begin()) {
      i--; 
    }

    for (i; i != ranges_.end(); ++i) { 
      if (i->second.start > end) { 
	return results; 
      }
      if ((i->second.end > start) and (i->second.start <= end)) { 
	results.push_back(i->second.val); 
      }
      
      
    }
    
    return results; 
    
  }

private:
  struct Range {
    timeid_t start; 
    timeid_t end; 
    T val; 
  }; 
  
  typedef std::map<timeid_t, Range> rangemap_t; 
  rangemap_t ranges_;   
  
}; 


}

#endif
