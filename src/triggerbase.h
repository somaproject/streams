#ifndef TRIGGERBASE_H
#define TRIGGERBASE_H

#include <somanetwork/network.h>

#include <boost/shared_ptr.hpp>
#include <list>

#include <vector>
#include <sigc++/sigc++.h>
#include <somanetwork/wave.h>
#include <boost/shared_ptr.hpp>
#include <core/element.h>

#include "itrigger.h"
class TriggerBase : public core::Element,  public ITrigger
{
public:
  
  // name
  static const std::string TYPENAME; 

  TriggerBase(std::string name) : core::Element(name)
  {
  } 
  
  virtual ~TriggerBase() {}; 

}; 

#endif // TRIGGER!BASE_H

