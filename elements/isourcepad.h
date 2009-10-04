#ifndef __ELEMENTS_ISOURCEPAD_H__
#define __ELEMENTS_ISOURCEPAD_H__
#include <boost/shared_ptr.hpp>

#include <elements/types.h>

namespace elements { 
  class ISinkPad; 
  typedef boost::shared_ptr<ISinkPad> pISinkPad_t; 

  class ISourcePad 
  {
  public:
    virtual std::string getName() = 0; 
    virtual void connect(pISinkPad_t) = 0; 
  }; 
  template<class T> 
  class SourcePad; 
  
  typedef ISourcePad * pISourcePad_t; 

}

#endif // ISOURCEPAD_H
