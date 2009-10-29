#ifndef __ELEMENTS_TYPES_H__
#define __ELEMENTS_TYPES_H__

#include <stdint.h>
#include <utility>
#include <list>
#include <itl/interval.hpp>
namespace elements {

typedef int64_t timeid_t; 

static const timeid_t TIMEID_NS = 1; 
static const timeid_t TIMEID_US = 1000; 
static const timeid_t TIMEID_MS = 1000000; 
static const timeid_t TIMEID_SEC = 1000000000; 


const static int64_t TIMEID_PER_SEC = 1000000000; 
const static double TIMEID_PER_SECF = 1000000000.0; 

typedef boost::itl::interval<timeid_t> timeinterval_t; 

struct timewindow_t
{
public:
  timewindow_t(timeid_t s, timeid_t e) :
    start(s),
    end(e)
  {
    
  }

  timewindow_t() :
    start(0),
    end(0)
  {

  }

  timeid_t start; 
  timeid_t end; 
  
  timeinterval_t interval() {
    return timeinterval_t(start, end);     
  }

}; 

/* Hashes are of wrapped data? */ 

template<typename T> 
struct datawindow_t
{
  size_t sequenceid; // change to signal reset. 
  std::list<T> data; 
  timeinterval_t interval; 
}; 

typedef size_t padid_t; 



}
#endif 
