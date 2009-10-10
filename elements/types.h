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

typedef std::pair<timeid_t, timeid_t> timewindow_t; 

} 

#endif 
