#ifndef __ELEMENTS_TYPES_H__
#define __ELEMENTS_TYPES_H__

#include <stdint.h>
#include <utility>

namespace elements {

typedef int64_t timeid_t; 

static const timeid_t TIMEID_NS = 1; 
static const timeid_t TIMEID_US = 1000; 
static const timeid_t TIMEID_MS = 1000000; 
static const timeid_t TIMEID_SEC = 1000000000; 


const static int64_t TIMEID_PER_SEC = 1000000000; 
const static double TIMEID_PER_SECF = 1000000000.0; 

; 

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
}; 


typedef size_t padid_t; 

template<typename T> 
class IElementSource
{
public:
  virtual void get_src_data(std::list<T> &, padid_t, const timewindow_t &) = 0; 
}; 

} 

#endif 
