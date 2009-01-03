#ifndef ITRIGGER_H
#define ITRIGGER_H
#include <boost/shared_ptr.hpp>
#include <map>
#include <iostream>

//#include "wave.h"

#include "core/ielement.h" 


// this is where we typedef the relevent typedefs
typedef sigc::signal<void, bool> updateTriggersSignal_t; 


class ITrigger : public virtual core::IElement
{
  
public:
  
}; 

typedef boost::shared_ptr<ITrigger> pITrigger_t; 

#endif // ITRIGGER_H
