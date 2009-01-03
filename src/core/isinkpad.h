#ifndef ISINKPAD_H
#define ISINKPAD_H
#include <boost/shared_ptr.hpp>

namespace core { 
  class ISinkPad 
  {
  public:
    virtual std::string getName() = 0; 

  }; 

  // forward declaration
  template<class T> 
  class SinkPad; 
  
  typedef ISinkPad * pISinkPad_t;
}

#endif // ISINKPAD_H
