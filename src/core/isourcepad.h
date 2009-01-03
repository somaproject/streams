#ifndef ISOURCEPAD_H
#define ISOURCEPAD_H
#include <boost/shared_ptr.hpp>

namespace core { 
  class ISinkPad; 

  class ISourcePad 
  {
  public:
    virtual std::string getName() = 0; 
    virtual void connect(ISinkPad *) = 0; 

  }; 
  template<class T> 
  class SourcePad; 
  
  typedef ISourcePad * pISourcePad_t; 

}

#endif // ISOURCEPAD_H
