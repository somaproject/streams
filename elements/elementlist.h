#ifndef __ELEMENTS_LIST_H__
#define __ELEMENTS_LIST_H__

#include <elements/types.h>

namespace elements {
  
  template<typename T>
  class LinkElement {
    
  public:
    typedef boost::shared_ptr<LinkElement> ptr_t; 

    enum STATES {
      RESET,
      DATA
    }; 
    // note that datum is only valid for data, sequencehead
    STATES state; 
    timeid_t starttime; 
    timeid_t endtime; 
    T datum; 
    
  }; 

  


}


#endif 
