#ifndef ITRIGGERVISITOR_H
#define ITRIGGERVISITOR_H

#include "triggerbase.h"
#include "data/wave.h"

class ITriggerVisitor {
public:
  virtual void visit(TriggerBase  * ) = 0; 
  //virtual void visi
  
}; 

typedef boost::shared_ptr<ITriggerVisitor> pITriggerVisitor_t; 

#endif //ITRIGGERVISITOR_H
