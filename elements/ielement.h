#ifndef __ELEMENTS_IELEMENT_H__
#define __ELEMENTS_IELEMENT_H__

#include <elements/types.h>

#include <elements/sourcepad.h>

namespace elements {
  class IElement {
  public:
    virtual std::string getName()  = 0; 
    virtual void setName(std::string name) =0; 
    
    virtual pISourcePad_t getSourcePad(std::string name) = 0; 
    virtual pISinkPad_t getSinkPad(std::string name) = 0; 

    virtual void process(timeid_t) = 0; 

  }; 

  typedef boost::shared_ptr<IElement> pIElement_t; 
}
  

#endif // IELEMENT_H
