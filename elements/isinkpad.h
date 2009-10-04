#ifndef __ELEMENTS_ISINKPAD_H__
#define __ELEMENTS_ISINKPAD_H__
#include <boost/shared_ptr.hpp>

namespace elements { 
  class ISinkPad 
  {
  public:
    virtual std::string getName() = 0; 
    virtual void send(MESSAGE m) = 0; 
  }; 

  // forward declaration
  template<class T> 
  class SinkPad; 
  
  typedef ISinkPad * pISinkPad_t;
}

#endif // ISINKPAD_H
