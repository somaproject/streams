#ifndef __ELEMENTS_TYPES_H__
#define __ELEMENTS_TYPES_H__

namespace elements {

typedef int64_t timeid_t; 

const static int64_t TIMEID_PER_SEC = 1000000000; 
const static double TIMEID_PER_SECF = 1000000000.0; 

; 

typedef std::pair<timeid_t, timeid_t> timewindow_t; 

} 

#endif 
