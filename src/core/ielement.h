#ifndef IELEMENT_H
#define IELEMENT_H
#include "sourcepad.h"

namespace core {
  class IElement {
  public:
    virtual std::string getName()  = 0; 
    virtual void setName(std::string name) =0; 
    
    virtual pISourcePad_t getSourcePad(std::string name) = 0; 
    virtual pISinkPad_t getSinkPad(std::string name) = 0; 
  }; 

  typedef boost::shared_ptr<IElement> pIElement_t; 
}
  

#endif // IELEMENT_H
