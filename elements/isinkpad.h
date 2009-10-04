#ifndef __ELEMENTS_ISINKPAD_H__
#define __ELEMENTS_ISINKPAD_H__
#include <boost/shared_ptr.hpp>

#include <elements/commands.h> 
#include <elements/elementlist.h>

namespace elements { 
  class ISinkPad 
  {
  public:
    virtual std::string getName() = 0; 
    virtual void sendmsg(MESSAGES m) = 0; 
  }; 

  // forward declaration
  template<class T> 
  class SinkPad; 
  
  typedef boost::shared_ptr<ISinkPad>  pISinkPad_t;
}

#endif // ISINKPAD_H
